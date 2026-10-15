// swift-tools-version: 5.9
import PackageDescription

let package = Package(
    name: "Tuck",
    platforms: [.macOS(.v14)],
    products: [.executable(name: "Tuck", targets: ["Tuck"])],
    targets: [
        .target(
            name: "ArchiveCore",
            publicHeadersPath: "include",
            linkerSettings: [.linkedLibrary("archive.2")]
        ),
        .executableTarget(name: "Tuck", dependencies: ["ArchiveCore"])
    ],
    swiftLanguageVersions: [.v5]
)
