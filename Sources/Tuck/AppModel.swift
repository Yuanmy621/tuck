import AppKit
import Combine
import Foundation
import UniformTypeIdentifiers

enum ArchiveMode: String, CaseIterable, Identifiable {
    case compress, extract
    var id: String { rawValue }
    var title: String { self == .compress ? "压缩文件" : "解压缩" }
}

struct ArchiveOutcome: Identifiable {
    enum State { case success, failed, cancelled }
    let id = UUID()
    let state: State
    let title: String
    let detail: String
    let destination: URL?
}

struct ArchivePasswordPrompt: Identifiable {
    let id = UUID()
    let archiveName: String
    let isRetry: Bool
}

private struct ArchiveJob {
    enum Operation {
        case compress(inputs: [URL], destination: URL, format: ArchiveFormat)
        case extract(archive: URL, parent: URL)
    }
    let id: UUID
    let operation: Operation
    var password: String?
    var passwordAttempts: Int
    var title: String {
        switch operation {
        case .compress(_, let destination, _): return destination.lastPathComponent
        case .extract(let archive, _): return archive.lastPathComponent
        }
    }
    var verb: String {
        switch operation {
        case .compress: return "压缩"
        case .extract: return "解压"
        }
    }
}

@MainActor
final class AppModel: ObservableObject {
    static let shared = AppModel()

    @Published var mode: ArchiveMode = .compress
    @Published var format: ArchiveFormat = .zip
    @Published private(set) var inputs: [URL] = []
    @Published private(set) var isProcessing = false
    @Published private(set) var progressFraction: Double?
    @Published private(set) var progressMessage = ""
    @Published private(set) var currentName = ""
    @Published private(set) var queuedCount = 0
    @Published private(set) var outcomes: [ArchiveOutcome] = []
    @Published var errorMessage: String?
    @Published var passwordPrompt: ArchivePasswordPrompt?
    @Published var revealWhenFinished = true
    @Published private(set) var associationMessage: String?
    @Published private(set) var associationFailed = false
    @Published private(set) var associationBusy = false

    private var jobs: [ArchiveJob] = []
    private var activeJob: ArchiveJob?
    private var activeRunID: UUID?
    private var activeCancellation: ArchiveCancellation?
    private var activeTask: Task<Void, Never>?
    private var resultsToReveal: [URL] = []
    private var isCancelling = false

    var actionTitle: String {
        if mode == .compress { return "压缩并保存…" }
        return inputs.count > 1 ? "解压 \(inputs.count) 个压缩包" : "开始解压"
    }

    func addInputs(_ urls: [URL]) {
        guard !isProcessing else { return }
        let valid = localExistingURLs(urls)
        guard !valid.isEmpty else { return }
        var known = Set(inputs.map(\.path))
        for url in valid where known.insert(url.path).inserted { inputs.append(url) }
        mode = inputs.allSatisfy(ArchiveEngine.isArchive) ? .extract : .compress
        errorMessage = nil
    }

    /// Finder's Open With / double-click entry point. Archive opens start immediately.
    func acceptOpenedURLs(_ urls: [URL]) {
        let valid = localExistingURLs(urls)
        let archives = valid.filter(ArchiveEngine.isArchive)
        let files = valid.filter { !ArchiveEngine.isArchive($0) }
        if !files.isEmpty {
            if !isProcessing { addInputs(files); mode = .compress }
            else { errorMessage = "正在处理压缩包。请完成后再添加要压缩的文件。" }
        }
        guard !archives.isEmpty else { return }
        mode = .extract
        for url in archives {
            jobs.append(ArchiveJob(id: UUID(), operation: .extract(archive: url, parent: url.deletingLastPathComponent()), password: nil, passwordAttempts: 0))
        }
        queuedCount = jobs.count
        if !isProcessing { startNextJob() }
    }

    func removeInput(_ url: URL) { inputs.removeAll { $0 == url } }
    func clearInputs() { inputs.removeAll(); errorMessage = nil }

    func chooseInputs() {
        guard !isProcessing else { return }
        let panel = NSOpenPanel()
        panel.title = mode == .compress ? "选择要压缩的文件或文件夹" : "选择要解压的压缩包"
        panel.prompt = "添加"
        panel.canChooseFiles = true
        panel.canChooseDirectories = mode == .compress
        panel.allowsMultipleSelection = true
        if panel.runModal() == .OK { addInputs(panel.urls) }
    }

    func startSelected() {
        guard !isProcessing, !inputs.isEmpty else { return }
        errorMessage = nil
        if mode == .extract {
            let unsupported = inputs.filter { !ArchiveEngine.isArchive($0) }
            guard unsupported.isEmpty else {
                errorMessage = "请选择受支持的压缩包。无法解压：" + unsupported.map(\.lastPathComponent).joined(separator: "、")
                return
            }
            jobs = inputs.map { ArchiveJob(id: UUID(), operation: .extract(archive: $0, parent: $0.deletingLastPathComponent()), password: nil, passwordAttempts: 0) }
            inputs.removeAll()
            startNextJob()
        } else {
            chooseCompressionDestination()
        }
    }

    private func chooseCompressionDestination() {
        let panel = NSSavePanel()
        panel.title = "保存压缩包"
        panel.prompt = "开始压缩"
        panel.canCreateDirectories = true
        panel.isExtensionHidden = false
        panel.allowedContentTypes = [format == .zip ? .zip : .gzip]
        let baseName = inputs.count == 1 ? inputs[0].deletingPathExtension().lastPathComponent : "归档"
        panel.nameFieldStringValue = "\(baseName).\(format.fileExtension)"
        panel.directoryURL = inputs.first?.deletingLastPathComponent()
        guard panel.runModal() == .OK, let destination = panel.url else { return }
        jobs = [ArchiveJob(id: UUID(), operation: .compress(inputs: inputs, destination: destination, format: format), password: nil, passwordAttempts: 0)]
        inputs.removeAll()
        startNextJob()
    }

    private func startNextJob() {
        queuedCount = jobs.count
        guard !jobs.isEmpty else {
            isProcessing = false
            activeJob = nil
            activeRunID = nil
            activeCancellation = nil
            activeTask = nil
            currentName = ""
            if revealWhenFinished, !resultsToReveal.isEmpty {
                NSWorkspace.shared.activateFileViewerSelecting(resultsToReveal)
            }
            resultsToReveal.removeAll()
            return
        }
        let job = jobs.removeFirst()
        queuedCount = jobs.count
        launch(job)
    }

    private func launch(_ job: ArchiveJob) {
        isProcessing = true
        isCancelling = false
        activeJob = job
        let runID = UUID()
        activeRunID = runID
        currentName = job.title
        progressMessage = "正在准备\(job.verb)…"
        progressFraction = nil
        let cancellation = ArchiveCancellation()
        activeCancellation = cancellation
        activeTask = Task { [weak self] in
            let result: Result<URL, Error> = await Task.detached(priority: .userInitiated) {
                do {
                    let report: @Sendable (ArchiveProgress) -> Void = { [weak self] value in
                        Task { @MainActor in
                            guard let self, self.activeRunID == runID, !self.isCancelling else { return }
                            if let fraction = value.fraction, fraction.isFinite {
                                self.progressFraction = min(1, max(0, fraction))
                            } else {
                                self.progressFraction = nil
                            }
                            self.progressMessage = value.message
                        }
                    }
                    let output: URL
                    switch job.operation {
                    case .compress(let inputs, let destination, let format):
                        output = try ArchiveEngine.compress(inputs: inputs, destination: destination, format: format, password: nil, cancellation: cancellation, progress: report)
                    case .extract(let archive, let parent):
                        output = try ArchiveEngine.extract(archive: archive, destinationParent: parent, password: job.password, cancellation: cancellation, progress: report)
                    }
                    return .success(output)
                } catch { return .failure(error) }
            }.value
            self?.finish(job, result: result)
        }
    }

    private func finish(_ job: ArchiveJob, result: Result<URL, Error>) {
        guard activeJob?.id == job.id else { return }
        activeRunID = nil
        activeTask = nil
        activeCancellation = nil
        switch result {
        case .success(let output):
            outcomes.insert(ArchiveOutcome(state: .success, title: "\(job.verb)完成 · \(job.title)", detail: output.path, destination: output), at: 0)
            resultsToReveal.append(output)
        case .failure(let error):
            if let archiveError = error as? ArchiveError {
                switch archiveError {
                case .passwordRequired:
                    passwordPrompt = ArchivePasswordPrompt(archiveName: job.title, isRetry: job.passwordAttempts > 0)
                    progressMessage = "等待输入压缩包密码"
                    return
                case .cancelled:
                    outcomes.insert(ArchiveOutcome(state: .cancelled, title: "已取消 · \(job.title)", detail: "此项任务已停止。", destination: nil), at: 0)
                default:
                    recordFailure(job, error: error)
                }
            } else { recordFailure(job, error: error) }
        }
        outcomes = Array(outcomes.prefix(8))
        startNextJob()
    }

    private func recordFailure(_ job: ArchiveJob, error: Error) {
        let detail = error.localizedDescription
        errorMessage = "\(job.title)：\(detail)"
        outcomes.insert(ArchiveOutcome(state: .failed, title: "\(job.verb)失败 · \(job.title)", detail: detail, destination: nil), at: 0)
    }

    func submitPassword(_ password: String) {
        guard !password.isEmpty, var job = activeJob, passwordPrompt != nil else { return }
        job.password = password
        job.passwordAttempts += 1
        passwordPrompt = nil
        launch(job)
    }

    func cancelCurrent() {
        guard isProcessing else { return }
        if passwordPrompt != nil, let job = activeJob {
            passwordPrompt = nil
            finish(job, result: .failure(ArchiveError.cancelled))
        } else {
            isCancelling = true
            progressMessage = "正在取消并清理…"
            activeCancellation?.cancel()
        }
    }

    func cancelAll() {
        jobs.removeAll()
        queuedCount = 0
        cancelCurrent()
    }

    func reveal(_ url: URL) { NSWorkspace.shared.activateFileViewerSelecting([url]) }

    func makeDefault() {
        guard !associationBusy else { return }
        associationBusy = true
        Task {
            let failures = await ArchiveAssociations.makeDefault()
            associationFailed = !failures.isEmpty
            associationMessage = failures.isEmpty ? "已设为默认解压工具。现在可在 Finder 中双击支持的压缩包。" : failures.joined(separator: "\n")
            associationBusy = false
        }
    }

    func restoreDefaults() {
        guard !associationBusy else { return }
        associationBusy = true
        Task {
            let failures = await ArchiveAssociations.restoreDefaults()
            associationFailed = !failures.isEmpty
            associationMessage = failures.isEmpty ? "已恢复此前保存的默认打开工具。" : failures.joined(separator: "\n")
            associationBusy = false
        }
    }

    private func localExistingURLs(_ urls: [URL]) -> [URL] {
        urls.compactMap { url in
            guard url.isFileURL else { return nil }
            let normalized = url.standardizedFileURL
            guard FileManager.default.fileExists(atPath: normalized.path) else {
                errorMessage = "文件不存在或暂时无法访问：\(url.path)"
                return nil
            }
            return normalized
        }
    }
}
