import AppKit
import Foundation

func cliMain(_ args: [String]) async -> Int32 {
    let token = ArchiveCancellation()
    do {
        guard let command = args.first else { throw ArchiveError.failed("缺少命令。") }
        var result: [String: Any] = [:]
        switch command {
        case "extract":
            guard args.count == 3 || args.count == 5 else { throw ArchiveError.failed("extract ARCHIVE PARENT [--password PASSWORD]") }
            let password = args.count == 5 && args[3] == "--password" ? args[4] : nil
            let output = try ArchiveEngine.extract(
                archive: URL(fileURLWithPath: args[1]), destinationParent: URL(fileURLWithPath: args[2]),
                password: password, cancellation: token, progress: { _ in }
            )
            result = ["ok": true, "output": output.path]
        case "compress":
            guard args.count >= 4, let format = ArchiveFormat(rawValue: args[1]) else { throw ArchiveError.failed("compress zip|tarGzip OUTPUT INPUT...") }
            let output = try ArchiveEngine.compress(
                inputs: args.dropFirst(3).map { URL(fileURLWithPath: $0) }, destination: URL(fileURLWithPath: args[2]),
                format: format, password: nil, cancellation: token, progress: { _ in }
            )
            result = ["ok": true, "output": output.path]
        case "cancel-extract":
            guard args.count == 3 else { throw ArchiveError.failed("cancel-extract ARCHIVE PARENT") }
            token.cancel()
            _ = try ArchiveEngine.extract(archive: URL(fileURLWithPath: args[1]), destinationParent: URL(fileURLWithPath: args[2]), password: nil, cancellation: token, progress: { _ in })
            throw ArchiveError.failed("取消没有生效。")
        case "make-default":
            let errors = await ArchiveAssociations.makeDefault()
            result = ["ok": errors.isEmpty, "errors": errors, "handlers": ArchiveAssociations.currentHandlers()]
        case "restore-defaults":
            let errors = await ArchiveAssociations.restoreDefaults()
            result = ["ok": errors.isEmpty, "errors": errors, "handlers": ArchiveAssociations.currentHandlers()]
        case "associations":
            result = ["ok": true, "handlers": ArchiveAssociations.currentHandlers()]
        default:
            throw ArchiveError.failed("未知命令：\(command)")
        }
        let data = try JSONSerialization.data(withJSONObject: result, options: [.sortedKeys])
        print(String(decoding: data, as: UTF8.self))
        return result["ok"] as? Bool == false ? 1 : 0
    } catch {
        var kind = "failure"
        if let error = error as? ArchiveError {
            switch error { case .passwordRequired: kind = "passwordRequired"; case .cancelled: kind = "cancelled"; case .failed: break }
        }
        let record: [String: Any] = ["ok": false, "kind": kind, "error": error.localizedDescription]
        if let data = try? JSONSerialization.data(withJSONObject: record, options: [.sortedKeys]) {
            print(String(decoding: data, as: UTF8.self))
        }
        return 1
    }
}

if CommandLine.arguments.dropFirst().first == "--cli" {
    exit(await cliMain(Array(CommandLine.arguments.dropFirst(2))))
} else {
    TuckApplication.main()
}
