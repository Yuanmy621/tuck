import AppKit
import CoreServices
import UniformTypeIdentifiers

enum ArchiveAssociations {
    // Preserve the existing application identity and the first default-handler backup across the Tuck rename.
    static let bundleID = "local.ymy.MacArchive"
    static let extensions = ["zip", "7z", "rar", "tar", "gz", "gzip", "tgz", "bz", "bz2", "tbz", "tbz2", "xz", "txz"]

    private struct PreviousHandler: Codable {
        let type: String
        let handler: String?
    }

    private static var backupURL: URL {
        FileManager.default.urls(for: .applicationSupportDirectory, in: .userDomainMask)[0]
            .appendingPathComponent("MacArchive", isDirectory: true)
            .appendingPathComponent("previous-defaults.json")
    }

    static var types: [String] {
        Array(Set(extensions.compactMap { UTType(filenameExtension: $0)?.identifier })).sorted()
    }

    static func currentHandlers() -> [String: String] {
        Dictionary(uniqueKeysWithValues: types.map { type in
            (type, currentHandler(type) ?? "")
        })
    }

    private static func currentHandler(_ identifier: String) -> String? {
        guard let type = UTType(identifier),
              let url = NSWorkspace.shared.urlForApplication(toOpen: type) else { return nil }
        return Bundle(url: url)?.bundleIdentifier
    }

    static func makeDefault() async -> [String] {
        guard Bundle.main.bundleIdentifier == bundleID,
              Bundle.main.bundleURL.pathExtension == "app" else {
            return ["请从 Applications 中打开 Tuck，再设置默认打开方式。"]
        }
        let registration = LSRegisterURL(Bundle.main.bundleURL as CFURL, true)
        guard registration == noErr else { return ["无法注册应用（系统错误 \(registration)）。"] }
        do {
            // Preserve the first snapshot; repeat setup must never replace the user's old handlers.
            let parent = backupURL.deletingLastPathComponent()
            try FileManager.default.createDirectory(at: parent, withIntermediateDirectories: true)
            if !FileManager.default.fileExists(atPath: backupURL.path) {
                let previous = types.map { type in
                    PreviousHandler(type: type, handler: currentHandler(type))
                }
                let data = try JSONEncoder().encode(previous)
                try data.write(to: backupURL, options: .withoutOverwriting)
            }
        } catch {
            return ["无法备份原来的默认打开方式：\(error.localizedDescription)"]
        }
        var failures: [String] = []
        for type in types {
            if let error = await setHandler(Bundle.main.bundleURL, identifier: bundleID, type: type) { failures.append(error) }
        }
        return failures
    }

    static func restoreDefaults() async -> [String] {
        do {
            let previous = try JSONDecoder().decode([PreviousHandler].self, from: Data(contentsOf: backupURL))
            var failures: [String] = []
            for item in previous {
                guard let handler = item.handler else {
                    if currentHandler(item.type)?.lowercased() == bundleID.lowercased() {
                        failures.append("\(item.type) 原来没有默认工具，请在 Finder 的‘显示简介’中选择需要的工具。")
                    }
                    continue
                }
                guard let url = NSWorkspace.shared.urlForApplication(withBundleIdentifier: handler) else {
                    failures.append("找不到原默认工具 \(handler)。")
                    continue
                }
                if let error = await setHandler(url, identifier: handler, type: item.type) { failures.append(error) }
            }
            return failures
        } catch { return ["无法读取默认打开方式备份：\(error.localizedDescription)"] }
    }

    private static func setHandler(_ application: URL, identifier: String, type identifierType: String) async -> String? {
        guard let type = UTType(identifierType) else { return "无法识别文件类型：\(identifierType)" }
        do {
            try await NSWorkspace.shared.setDefaultApplication(at: application, toOpen: type)
            guard currentHandler(identifierType)?.lowercased() == identifier.lowercased() else {
                return "\(identifierType) 的默认打开方式尚未生效，请在 Finder 的‘显示简介’中设置。"
            }
            return nil
        } catch { return "\(identifierType)：\(error.localizedDescription)" }
    }
}
