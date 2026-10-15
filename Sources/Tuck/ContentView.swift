import AppKit
import SwiftUI
import UniformTypeIdentifiers

private enum ArchivePalette {
    static let paper = Color(red: 0.965, green: 0.953, blue: 0.930)
    static let ink = Color(red: 0.20, green: 0.22, blue: 0.21)
    static let secondary = Color(red: 0.45, green: 0.46, blue: 0.43)
    static let accent = Color(red: 0.82, green: 0.34, blue: 0.14)
    static let line = Color(red: 0.87, green: 0.85, blue: 0.81)
}

struct ContentView: View {
    @ObservedObject var model: AppModel
    @State private var dropTargeted = false

    var body: some View {
        VStack(spacing: 0) {
            ScrollView {
                VStack(alignment: .leading, spacing: 21) {
                    header
                    workspace
                    if let error = model.errorMessage { errorBanner(error) }
                    if !model.outcomes.isEmpty { recentActivity }
                    defaultTool
                }
                .padding(28)
            }
            footer
        }
        .background(ArchivePalette.paper)
        .foregroundStyle(ArchivePalette.ink)
        .tint(ArchivePalette.accent)
        .sheet(item: $model.passwordPrompt, onDismiss: {
            // Interactive sheet dismissal must not leave the queue waiting forever.
            if model.isProcessing && model.progressMessage == "等待输入压缩包密码" { model.cancelCurrent() }
        }) { prompt in
            ArchivePasswordView(prompt: prompt, onSubmit: model.submitPassword, onCancel: model.cancelCurrent)
                .interactiveDismissDisabled()
        }
    }

    private var header: some View {
        HStack(alignment: .center, spacing: 13) {
            ZStack {
                RoundedRectangle(cornerRadius: 15).fill(ArchivePalette.accent)
                Image(systemName: "archivebox.fill")
                    .font(.system(size: 27, weight: .medium))
                    .foregroundStyle(.white)
            }
            .frame(width: 58, height: 58)
            VStack(alignment: .leading, spacing: 4) {
                Text("Tuck").font(.system(size: 28, weight: .semibold, design: .rounded))
                Text("轻松打包，从容展开。")
                    .font(.system(size: 12))
                    .foregroundStyle(ArchivePalette.secondary)
            }
            Spacer()
            Label("本机处理", systemImage: "desktopcomputer")
                .font(.system(size: 11, weight: .medium))
                .foregroundStyle(ArchivePalette.secondary)
                .padding(.horizontal, 11).padding(.vertical, 7)
                .background(.white.opacity(0.65), in: Capsule())
        }
    }

    private var workspace: some View {
        VStack(alignment: .leading, spacing: 18) {
            HStack {
                Picker("操作", selection: $model.mode) {
                    ForEach(ArchiveMode.allCases) { mode in Text(mode.title).tag(mode) }
                }
                .pickerStyle(.segmented)
                .frame(width: 220)
                .disabled(model.isProcessing)
                Spacer()
                if !model.inputs.isEmpty {
                    Text("已选 \(model.inputs.count) 项").font(.system(size: 11)).foregroundStyle(ArchivePalette.secondary)
                    Button("清空", action: model.clearInputs).buttonStyle(.plain)
                        .font(.system(size: 11)).disabled(model.isProcessing)
                }
            }

            VStack(spacing: 10) {
                Image(systemName: model.mode == .compress ? "square.and.arrow.down.on.square" : "shippingbox")
                    .font(.system(size: 31, weight: .light))
                    .foregroundStyle(ArchivePalette.accent)
                Text(model.inputs.isEmpty ? "把文件拖到这里" : "还可以继续拖入文件")
                    .font(.system(size: 18, weight: .medium))
                Text(model.mode == .compress ? "文件或文件夹，打包成一个压缩包" : "解压到原位置旁的独立文件夹")
                    .font(.system(size: 12)).foregroundStyle(ArchivePalette.secondary)
                Button("选择文件…", action: model.chooseInputs)
                    .buttonStyle(.bordered)
                    .controlSize(.regular)
                    .padding(.top, 2)
                    .disabled(model.isProcessing)
            }
            .frame(maxWidth: .infinity)
            .padding(.vertical, model.inputs.isEmpty ? 26 : 18)
            .background(dropTargeted ? ArchivePalette.accent.opacity(0.08) : ArchivePalette.paper.opacity(0.55), in: RoundedRectangle(cornerRadius: 16))
            .overlay {
                RoundedRectangle(cornerRadius: 16)
                    .strokeBorder(dropTargeted ? ArchivePalette.accent : ArchivePalette.line, style: StrokeStyle(lineWidth: 1.3, dash: [5, 5]))
            }
            .onDrop(of: [UTType.fileURL.identifier], isTargeted: $dropTargeted, perform: acceptDrop)

            if !model.inputs.isEmpty {
                ScrollView {
                    VStack(spacing: 0) {
                        ForEach(model.inputs, id: \.path) { url in
                            inputRow(url)
                            if url != model.inputs.last { Divider().padding(.leading, 41) }
                        }
                    }
                }
                .frame(height: CGFloat(min(model.inputs.count, 3)) * 48)
            }

            if model.mode == .compress {
                HStack(spacing: 14) {
                    Text("压缩格式").font(.system(size: 12, weight: .medium))
                    Picker("压缩格式", selection: $model.format) {
                        ForEach(ArchiveFormat.allCases) { format in Text(format.displayName).tag(format) }
                    }
                    .labelsHidden()
                    .pickerStyle(.segmented).frame(width: 190)
                    .disabled(model.isProcessing)
                    Spacer()
                    Text("保存时选择位置")
                        .font(.system(size: 11)).foregroundStyle(ArchivePalette.secondary)
                }
            } else {
                Label("每个压缩包使用独立文件夹，避免覆盖已有文件。", systemImage: "folder.badge.plus")
                    .font(.system(size: 12)).foregroundStyle(ArchivePalette.secondary)
            }
        }
        .padding(21)
        .background(.white.opacity(0.87), in: RoundedRectangle(cornerRadius: 22))
        .overlay { RoundedRectangle(cornerRadius: 22).strokeBorder(.white, lineWidth: 1) }
    }

    private func inputRow(_ url: URL) -> some View {
        HStack(spacing: 10) {
            Image(nsImage: NSWorkspace.shared.icon(forFile: url.path))
                .resizable().frame(width: 29, height: 29)
            VStack(alignment: .leading, spacing: 3) {
                Text(url.lastPathComponent).font(.system(size: 12, weight: .medium)).lineLimit(1)
                Text(url.deletingLastPathComponent().path)
                    .font(.system(size: 10)).foregroundStyle(ArchivePalette.secondary)
                    .lineLimit(1).truncationMode(.middle)
            }
            Spacer(minLength: 8)
            Button { model.removeInput(url) } label: { Image(systemName: "xmark.circle.fill").foregroundStyle(ArchivePalette.secondary.opacity(0.7)) }
                .buttonStyle(.plain).disabled(model.isProcessing)
                .help("移除 \(url.lastPathComponent)")
                .accessibilityLabel("移除 \(url.lastPathComponent)")
        }
        .frame(height: 48)
        .help(url.path)
    }

    private func errorBanner(_ error: String) -> some View {
        HStack(alignment: .top, spacing: 9) {
            Image(systemName: "exclamationmark.triangle.fill").foregroundStyle(ArchivePalette.accent)
            Text(error).font(.system(size: 12)).textSelection(.enabled).fixedSize(horizontal: false, vertical: true)
            Spacer(minLength: 0)
            Button { model.errorMessage = nil } label: { Image(systemName: "xmark") }
                .buttonStyle(.plain).accessibilityLabel("关闭错误提示")
        }
        .padding(13)
        .background(ArchivePalette.accent.opacity(0.07), in: RoundedRectangle(cornerRadius: 12))
    }

    private var recentActivity: some View {
        VStack(alignment: .leading, spacing: 11) {
            Text("最近完成").font(.system(size: 11, weight: .semibold)).foregroundStyle(ArchivePalette.secondary)
            ForEach(Array(model.outcomes.prefix(3))) { outcome in
                HStack(alignment: .center, spacing: 10) {
                    Image(systemName: outcome.state == .success ? "checkmark.circle.fill" : outcome.state == .cancelled ? "minus.circle" : "exclamationmark.circle.fill")
                        .font(.system(size: 17))
                        .foregroundStyle(outcome.state == .success ? Color(red: 0.32, green: 0.48, blue: 0.35) : ArchivePalette.secondary)
                    VStack(alignment: .leading, spacing: 3) {
                        Text(outcome.title).font(.system(size: 12, weight: .medium)).lineLimit(1)
                        Text(outcome.detail).font(.system(size: 10)).foregroundStyle(ArchivePalette.secondary)
                            .lineLimit(1).truncationMode(.middle)
                    }
                    Spacer()
                    if let destination = outcome.destination {
                        Button { model.reveal(destination) } label: { Image(systemName: "arrow.up.right.square") }
                            .buttonStyle(.plain).help("在 Finder 中显示")
                            .accessibilityLabel("在 Finder 中显示 \(destination.lastPathComponent)")
                    }
                }
                .help(outcome.detail)
            }
        }
        .padding(.horizontal, 4)
    }

    private var defaultTool: some View {
        VStack(alignment: .leading, spacing: 9) {
            HStack(spacing: 12) {
                Image(systemName: "cursorarrow.click.2").font(.system(size: 22, weight: .light)).foregroundStyle(ArchivePalette.secondary)
                VStack(alignment: .leading, spacing: 4) {
                    Text("双击，即可解压").font(.system(size: 12, weight: .medium))
                    Text("设为默认工具后，在 Finder 中直接打开压缩包。")
                        .font(.system(size: 11)).foregroundStyle(ArchivePalette.secondary)
                }
                Spacer(minLength: 8)
                Button("设为默认解压工具", action: model.makeDefault)
                    .controlSize(.small).disabled(model.isProcessing || model.associationBusy)
            }
            HStack {
                Text("支持 ZIP、TAR、GZIP 等归档；具体格式以解压结果为准。")
                    .font(.system(size: 10)).foregroundStyle(ArchivePalette.secondary)
                Spacer()
                Button("恢复原默认工具", action: model.restoreDefaults)
                    .buttonStyle(.plain).font(.system(size: 10))
                    .foregroundStyle(ArchivePalette.secondary)
                    .disabled(model.isProcessing || model.associationBusy)
            }
            if let message = model.associationMessage {
                Label {
                    Text(message).textSelection(.enabled).fixedSize(horizontal: false, vertical: true)
                } icon: {
                    Image(systemName: model.associationFailed ? "exclamationmark.circle" : "checkmark.circle")
                }
                .font(.system(size: 11))
                .foregroundStyle(model.associationFailed ? ArchivePalette.accent : ArchivePalette.secondary)
            }
        }
        .padding(16)
        .background(.white.opacity(0.4), in: RoundedRectangle(cornerRadius: 15))
    }

    private var footer: some View {
        VStack(spacing: 12) {
            if model.isProcessing {
                HStack(alignment: .center, spacing: 14) {
                    VStack(alignment: .leading, spacing: 6) {
                        HStack {
                            Text(model.currentName).font(.system(size: 12, weight: .medium)).lineLimit(1)
                            Spacer()
                            if model.queuedCount > 0 { Text("另有 \(model.queuedCount) 项排队").font(.system(size: 10)).foregroundStyle(ArchivePalette.secondary) }
                        }
                        if let fraction = model.progressFraction {
                            ProgressView(value: fraction).progressViewStyle(.linear)
                        } else {
                            ProgressView().progressViewStyle(.linear)
                        }
                        Text(model.progressMessage).font(.system(size: 10)).foregroundStyle(ArchivePalette.secondary).lineLimit(1)
                    }
                    Button(model.queuedCount > 0 ? "全部取消" : "取消", action: model.cancelAll)
                        .controlSize(.regular)
                }
            } else {
                HStack {
                    Toggle("完成后在 Finder 中显示", isOn: $model.revealWhenFinished)
                        .toggleStyle(.checkbox).font(.system(size: 11))
                        .foregroundStyle(ArchivePalette.secondary)
                    Spacer()
                    Button(model.actionTitle, action: model.startSelected)
                        .buttonStyle(.borderedProminent)
                        .controlSize(.large)
                        .disabled(model.inputs.isEmpty)
                        .keyboardShortcut(.return, modifiers: .command)
                }
            }
        }
        .padding(.horizontal, 28).padding(.vertical, 19)
        .background(.white.opacity(0.65))
        .overlay(alignment: .top) { Rectangle().fill(ArchivePalette.line.opacity(0.7)).frame(height: 1) }
    }

    private func acceptDrop(_ providers: [NSItemProvider]) -> Bool {
        guard !model.isProcessing else { return false }
        let files = providers.filter { $0.hasItemConformingToTypeIdentifier(UTType.fileURL.identifier) }
        guard !files.isEmpty else { return false }
        Task { @MainActor in
            var urls: [URL] = []
            for provider in files {
                let url: URL? = await withCheckedContinuation { continuation in
                    if provider.canLoadObject(ofClass: NSURL.self) {
                        provider.loadObject(ofClass: NSURL.self) { object, _ in
                            continuation.resume(returning: (object as? NSURL).map { $0 as URL })
                        }
                    } else {
                        provider.loadDataRepresentation(forTypeIdentifier: UTType.fileURL.identifier) { data, _ in
                            continuation.resume(returning: data.flatMap { URL(dataRepresentation: $0, relativeTo: nil) })
                        }
                    }
                }
                if let url { urls.append(url) }
            }
            model.addInputs(urls)
        }
        return true
    }
}

private struct ArchivePasswordView: View {
    let prompt: ArchivePasswordPrompt
    let onSubmit: (String) -> Void
    let onCancel: () -> Void
    @State private var password = ""
    @FocusState private var isFocused: Bool

    var body: some View {
        VStack(alignment: .leading, spacing: 16) {
            Image(systemName: "lock.shield").font(.system(size: 30, weight: .light)).foregroundStyle(ArchivePalette.accent)
            Text("这个压缩包需要密码").font(.system(size: 20, weight: .semibold))
            Text(prompt.archiveName).font(.system(size: 12)).foregroundStyle(ArchivePalette.secondary).lineLimit(2)
            if prompt.isRetry {
                Text("密码未通过，或此归档使用了不支持的加密方式。请检查后重试。")
                    .font(.system(size: 11)).foregroundStyle(ArchivePalette.accent)
            }
            SecureField("压缩包密码", text: $password)
                .textFieldStyle(.roundedBorder).focused($isFocused)
                .onSubmit(submit)
            Text("密码只用于本次解压，不会保存。")
                .font(.system(size: 10)).foregroundStyle(ArchivePalette.secondary)
            HStack {
                Spacer()
                Button("取消此项", action: onCancel).keyboardShortcut(.cancelAction)
                Button("解压", action: submit).buttonStyle(.borderedProminent)
                    .disabled(password.isEmpty).keyboardShortcut(.defaultAction)
            }
        }
        .padding(28).frame(width: 370)
        .background(ArchivePalette.paper)
        .onAppear { isFocused = true }
    }

    private func submit() {
        guard !password.isEmpty else { return }
        onSubmit(password)
        password = ""
    }
}
