import Foundation
import ArchiveCore

enum ArchiveFormat: String, CaseIterable, Identifiable {
    case zip, tarGzip
    var id: String { rawValue }
    var displayName: String { self == .zip ? "ZIP" : "TAR.GZ" }
    var fileExtension: String { self == .zip ? "zip" : "tar.gz" }
}

struct ArchiveProgress: Sendable {
    let fraction: Double?
    let message: String
}

final class ArchiveCancellation: @unchecked Sendable {
    private let lock = NSLock()
    private var cancelled = false
    func cancel() { lock.lock(); cancelled = true; lock.unlock() }
    var isCancelled: Bool { lock.lock(); defer { lock.unlock() }; return cancelled }
}

enum ArchiveError: Error, LocalizedError {
    case passwordRequired, cancelled, failed(String)
    var errorDescription: String? {
        switch self {
        case .passwordRequired: return "此压缩包需要密码，或密码不正确。请重新输入。"
        case .cancelled: return "操作已取消。"
        case .failed(let message): return message
        }
    }
}

private final class ArchiveCallbackContext {
    let cancellation: ArchiveCancellation
    let progress: @Sendable (ArchiveProgress) -> Void
    var lastUpdate: TimeInterval = 0
    init(cancellation: ArchiveCancellation, progress: @escaping @Sendable (ArchiveProgress) -> Void) {
        self.cancellation = cancellation
        self.progress = progress
    }
}

private let cancelCallback: @convention(c) (UnsafeMutableRawPointer?) -> Int32 = { pointer in
    guard let pointer else { return 1 }
    return Unmanaged<ArchiveCallbackContext>.fromOpaque(pointer).takeUnretainedValue().cancellation.isCancelled ? 1 : 0
}

private let progressCallback: @convention(c) (UnsafeMutableRawPointer?, Double, UnsafePointer<CChar>?) -> Void = { pointer, fraction, message in
    guard let pointer else { return }
    let context = Unmanaged<ArchiveCallbackContext>.fromOpaque(pointer).takeUnretainedValue()
    let now = ProcessInfo.processInfo.systemUptime
    guard now - context.lastUpdate >= 0.10 else { return }
    context.lastUpdate = now
    context.progress(ArchiveProgress(fraction: fraction < 0 ? nil : fraction, message: message.map(String.init(cString:)) ?? "处理中…"))
}

enum ArchiveEngine {
    private static let archiveExtensions: Set<String> = ["zip", "tar", "gz", "gzip", "tgz", "bz", "bz2", "bzip2", "tbz", "tbz2", "xz", "txz", "7z", "rar"]
    static func isArchive(_ url: URL) -> Bool {
        url.isFileURL && archiveExtensions.contains(url.pathExtension.lowercased())
    }

    static func compress(inputs: [URL], destination: URL, format: ArchiveFormat, password: String?,
                         cancellation: ArchiveCancellation, progress: @escaping @Sendable (ArchiveProgress) -> Void) throws -> URL {
        guard !inputs.isEmpty else { throw ArchiveError.failed("请选择要压缩的文件。") }
        try validateFileURLs(inputs + [destination])
        if cancellation.isCancelled { throw ArchiveError.cancelled }
        let parent = destination.deletingLastPathComponent().resolvingSymlinksInPath().standardizedFileURL
        let final = parent.appendingPathComponent(destination.lastPathComponent)
        // A staging file inside an input directory would be recursively included in its own output.
        for input in inputs {
            let resolved = input.resolvingSymlinksInPath().standardizedFileURL
            var directory: ObjCBool = false
            if FileManager.default.fileExists(atPath: resolved.path, isDirectory: &directory), directory.boolValue,
               parent.path == resolved.path || parent.path.hasPrefix(resolved.path + "/") {
                throw ArchiveError.failed("请把压缩包保存到所选文件夹之外，避免把输出文件再次压入自身。")
            }
        }
        let staging = try makeStaging(in: parent)
        var ownsStaging = true
        defer { if ownsStaging { try? removeStaging(staging) } }
        let output = staging.appendingPathComponent("archive.partial")
        do {
            let context = ArchiveCallbackContext(cancellation: cancellation, progress: progress)
            let result = try withCallbacks(context) { callbacks, error, size in
                let allocated = inputs.map { strdup($0.standardizedFileURL.path) }
                defer { allocated.forEach { free($0) } }
                guard allocated.allSatisfy({ $0 != nil }) else { throw ArchiveError.failed("内存不足。") }
                let pointers: [UnsafePointer<CChar>?] = allocated.map { value in value.map { UnsafePointer<CChar>($0) } }
                return pointers.withUnsafeBufferPointer { array in
                    ac_create_archive(array.baseAddress, array.count, output.path, format == .zip ? 0 : 1,
                                      password, callbacks, error, size)
                }
            }
            try checkResult(result)
            if cancellation.isCancelled { throw ArchiveError.cancelled }
            try finalize(output, to: final)
            try removeStaging(staging)
            ownsStaging = false
            progress(ArchiveProgress(fraction: 1, message: final.lastPathComponent))
            return final
        } catch {
            do { try removeStaging(staging); ownsStaging = false }
            catch let cleanup { throw ArchiveError.failed("\(error.localizedDescription) 临时文件清理失败，保留在 \(staging.path)：\(cleanup.localizedDescription)") }
            throw error
        }
    }

    static func extract(archive: URL, destinationParent: URL, password: String?,
                        cancellation: ArchiveCancellation, progress: @escaping @Sendable (ArchiveProgress) -> Void) throws -> URL {
        try validateFileURLs([archive, destinationParent])
        if cancellation.isCancelled { throw ArchiveError.cancelled }
        let parent = destinationParent.resolvingSymlinksInPath().standardizedFileURL
        let staging = try makeStaging(in: parent)
        var ownsStaging = true
        defer { if ownsStaging { try? removeStaging(staging) } }
        do {
            let context = ArchiveCallbackContext(cancellation: cancellation, progress: progress)
            let result = try withCallbacks(context) { callbacks, error, size in
                ac_extract_archive(archive.standardizedFileURL.path, staging.path, password, callbacks, error, size)
            }
            try checkResult(result)
            if cancellation.isCancelled { throw ArchiveError.cancelled }
            let base = extractedBaseName(archive.lastPathComponent)
            var suffix = 1
            while true {
                if cancellation.isCancelled { throw ArchiveError.cancelled }
                let name = suffix == 1 ? base : "\(base) (\(suffix))"
                let final = parent.appendingPathComponent(name, isDirectory: true)
                let result = try finalizeIfAvailable(staging, to: final)
                if result {
                    ownsStaging = false
                    progress(ArchiveProgress(fraction: 1, message: final.lastPathComponent))
                    return final
                }
                suffix += 1
            }
        } catch {
            do { try removeStaging(staging); ownsStaging = false }
            catch let cleanup { throw ArchiveError.failed("\(error.localizedDescription) 临时文件清理失败，保留在 \(staging.path)：\(cleanup.localizedDescription)") }
            throw error
        }
    }

    private struct CoreResult { let code: Int32; let message: String }
    private static func withCallbacks(_ context: ArchiveCallbackContext,
                                      operation: (ACCallbacks, UnsafeMutablePointer<CChar>, Int) throws -> Int32) throws -> CoreResult {
        var error = [CChar](repeating: 0, count: 4096)
        let callbacks = ACCallbacks(context: Unmanaged.passUnretained(context).toOpaque(), is_cancelled: cancelCallback, progress: progressCallback)
        let code = try withExtendedLifetime(context) {
            try error.withUnsafeMutableBufferPointer { buffer in
                try operation(callbacks, buffer.baseAddress!, buffer.count)
            }
        }
        return CoreResult(code: code, message: String(cString: error))
    }

    private static func checkResult(_ result: CoreResult) throws {
        switch result.code {
        case 0: return
        case 1: throw ArchiveError.passwordRequired
        case 2: throw ArchiveError.cancelled
        default: throw ArchiveError.failed(result.message.isEmpty ? "压缩引擎操作失败。" : result.message)
        }
    }

    private static func validateFileURLs(_ urls: [URL]) throws {
        guard urls.allSatisfy(\.isFileURL) else { throw ArchiveError.failed("仅支持本机文件。") }
        guard urls.allSatisfy({ !$0.path.utf8.contains(0) }) else { throw ArchiveError.failed("文件路径包含无效字符。") }
    }

    private static func makeStaging(in parent: URL) throws -> URL {
        var directory: ObjCBool = false
        guard FileManager.default.fileExists(atPath: parent.path, isDirectory: &directory), directory.boolValue else {
            throw ArchiveError.failed("目标文件夹不存在。")
        }
        let staging = parent.appendingPathComponent(".Tuck-\(UUID().uuidString).partial", isDirectory: true)
        try FileManager.default.createDirectory(at: staging, withIntermediateDirectories: false, attributes: [.posixPermissions: 0o700])
        return staging
    }

    private static func removeStaging(_ staging: URL) throws {
        var error = [CChar](repeating: 0, count: 4096)
        let result = error.withUnsafeMutableBufferPointer { buffer in
            ac_remove_staging(staging.path, buffer.baseAddress, buffer.count)
        }
        if result != 0 { throw ArchiveError.failed(String(cString: error)) }
    }

    private static func finalize(_ staging: URL, to destination: URL) throws {
        guard try finalizeIfAvailable(staging, to: destination) else {
            throw ArchiveError.failed("目标已存在，未覆盖：\(destination.lastPathComponent)")
        }
    }

    private static func finalizeIfAvailable(_ staging: URL, to destination: URL) throws -> Bool {
        var error = [CChar](repeating: 0, count: 4096)
        let result = error.withUnsafeMutableBufferPointer { buffer in
            ac_finalize_exclusive(staging.path, destination.path, buffer.baseAddress, buffer.count)
        }
        if result == 0 { return true }
        if result == 1 { return false }
        throw ArchiveError.failed(String(cString: error))
    }

    private static func extractedBaseName(_ name: String) -> String {
        let lower = name.lowercased()
        for suffix in [".tar.gzip", ".tar.bzip2", ".tar.gz", ".tar.bz2", ".tar.bz", ".tar.xz", ".tbz2", ".tgz", ".tbz", ".txz", ".zip", ".tar", ".gzip", ".bzip2", ".gz", ".bz2", ".bz", ".xz", ".7z", ".rar"] {
            if lower.hasSuffix(suffix) {
                let base = String(name.dropLast(suffix.count))
                return base.isEmpty || base == "." || base == ".." ? "解压文件" : base
            }
        }
        return name.isEmpty ? "解压文件" : name + " 解压"
    }
}
