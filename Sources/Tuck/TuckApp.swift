import AppKit
import Combine
import SwiftUI

@MainActor
final class TuckDelegate: NSObject, NSApplicationDelegate {
    private var terminationObservation: AnyCancellable?

    func applicationDidFinishLaunching(_ notification: Notification) {
        NSApp.setActivationPolicy(.regular)
        NSApp.activate(ignoringOtherApps: true)
    }

    func application(_ sender: NSApplication, openFiles filenames: [String]) {
        AppModel.shared.acceptOpenedURLs(filenames.map { URL(fileURLWithPath: $0) })
        sender.reply(toOpenOrPrint: .success)
        showWindow()
    }

    func application(_ application: NSApplication, open urls: [URL]) {
        AppModel.shared.acceptOpenedURLs(urls)
        showWindow()
    }

    func applicationShouldHandleReopen(_ sender: NSApplication, hasVisibleWindows flag: Bool) -> Bool {
        if !flag { showWindow() }
        return true
    }

    func applicationShouldTerminate(_ sender: NSApplication) -> NSApplication.TerminateReply {
        let model = AppModel.shared
        guard model.isProcessing else { return .terminateNow }
        guard terminationObservation == nil else { return .terminateLater }
        terminationObservation = model.$isProcessing
            .dropFirst()
            .filter { !$0 }
            .first()
            .sink { [weak self, weak sender] _ in
                // Reply after this delegate call returns, including immediate password cancellation.
                DispatchQueue.main.async {
                    sender?.reply(toApplicationShouldTerminate: true)
                    self?.terminationObservation = nil
                }
            }
        model.cancelAll()
        return .terminateLater
    }

    private func showWindow() {
        NSApp.activate(ignoringOtherApps: true)
        NSApp.windows.first(where: { $0.canBecomeMain })?.makeKeyAndOrderFront(nil)
    }
}

struct TuckApplication: App {
    @NSApplicationDelegateAdaptor(TuckDelegate.self) private var delegate
    @StateObject private var model = AppModel.shared

    var body: some Scene {
        WindowGroup("Tuck", id: "main") {
            ContentView(model: model)
                .frame(minWidth: 740, minHeight: 620)
                .preferredColorScheme(.light)
        }
        .defaultSize(width: 780, height: 730)
        .windowResizability(.contentMinSize)
        .commands {
            CommandGroup(replacing: .newItem) {
                Button("添加文件…") { model.chooseInputs() }
                    .keyboardShortcut("o")
                    .disabled(model.isProcessing)
            }
        }
    }
}
