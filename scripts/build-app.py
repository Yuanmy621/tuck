#!/usr/bin/env python3
"""Build a native app bundle with no package manager or third-party install."""
import os
import pathlib
import plistlib
import shutil
import subprocess
import sys
import uuid

ROOT = pathlib.Path(__file__).resolve().parent.parent
BUILD = ROOT / "build"
APP = BUILD / "Tuck.app"


def run(*args):
    subprocess.run(args, cwd=ROOT, check=True)


BUILD.mkdir(exist_ok=True)
run("swift", "build", "-c", "release", "--jobs", "2")
binpath = subprocess.check_output(["swift", "build", "-c", "release", "--show-bin-path"], cwd=ROOT, text=True).strip()
if APP.exists():
    shutil.rmtree(APP)
(APP / "Contents/MacOS").mkdir(parents=True)
(APP / "Contents/Resources").mkdir()
shutil.copy2(pathlib.Path(binpath) / "Tuck", APP / "Contents/MacOS/Tuck")
run("swift", str(ROOT / "scripts/make-icon.swift"), str(BUILD / "icon.png"))
iconset = BUILD / "AppIcon.iconset"
iconset.mkdir(exist_ok=True)
for size in [16, 32, 128, 256, 512]:
    for scale in [1, 2]:
        suffix = "@2x" if scale == 2 else ""
        run("sips", "-z", str(size * scale), str(size * scale), str(BUILD / "icon.png"), "--out", str(iconset / f"icon_{size}x{size}{suffix}.png"))
run("iconutil", "-c", "icns", str(iconset), "-o", str(APP / "Contents/Resources/AppIcon.icns"))

formats = [
    ("ZIP 压缩包", ["zip"], "public.zip-archive"),
    ("7-Zip 压缩包", ["7z"], "org.7-zip.7-zip-archive"),
    ("RAR 压缩包", ["rar"], "com.rarlab.rar-archive"),
    ("TAR 归档", ["tar"], "public.tar-archive"),
    ("Gzip 压缩文件", ["gz", "gzip", "tgz"], "org.gnu.gnu-zip-archive"),
    ("Bzip2 压缩文件", ["bz", "bz2", "tbz", "tbz2"], "public.bzip2-archive"),
    ("XZ 压缩文件", ["xz", "txz"], "org.tukaani.xz-archive"),
]
info = {
    "CFBundleName": "Tuck", "CFBundleDisplayName": "Tuck",
    "CFBundleIdentifier": "local.ymy.MacArchive", "CFBundleExecutable": "Tuck",
    "CFBundlePackageType": "APPL", "CFBundleShortVersionString": "1.0.0", "CFBundleVersion": "1",
    "CFBundleIconFile": "AppIcon", "LSMinimumSystemVersion": "14.0",
    "NSHighResolutionCapable": True, "NSPrincipalClass": "NSApplication",
    "NSHumanReadableCopyright": "Tuck · Uses macOS system libarchive",
    "CFBundleDevelopmentRegion": "zh_CN", "CFBundleLocalizations": ["zh_CN", "en"],
    "CFBundleDocumentTypes": [
        {"CFBundleTypeName": name, "CFBundleTypeRole": "Viewer", "LSHandlerRank": "Default",
         "CFBundleTypeExtensions": extensions, "LSItemContentTypes": [uti]}
        for name, extensions, uti in formats
    ],
    "UTImportedTypeDeclarations": [
        {"UTTypeIdentifier": uti, "UTTypeDescription": name,
         "UTTypeConformsTo": ["public.archive", "public.data"],
         "UTTypeTagSpecification": {"public.filename-extension": extensions}}
        for name, extensions, uti in formats
    ],
}
with (APP / "Contents/Info.plist").open("wb") as f:
    plistlib.dump(info, f)
run("codesign", "--force", "--sign", "-", str(APP))
run("codesign", "--verify", "--deep", "--strict", str(APP))

if "--install" in sys.argv:
    destination = pathlib.Path.home() / "Applications/Tuck.app"
    destination.parent.mkdir(exist_ok=True)
    legacy = destination.with_name("轻压.app")
    # Validate every affected app before moving any of them.
    existing = [path for path in [destination, legacy] if path.exists()]
    for path in existing:
        if plistlib.loads((path / "Contents/Info.plist").read_bytes()).get("CFBundleIdentifier") != info["CFBundleIdentifier"]:
            raise SystemExit(f"拒绝覆盖其他应用：{path}")
    for path in existing:
        backup = destination.parent / ".Tuck-backups" / f"{path.stem}-{uuid.uuid4().hex}.backup"
        backup.parent.mkdir(exist_ok=True)
        path.rename(backup)
        print(f"Previous application backed up: {backup}")
    shutil.copytree(APP, destination, symlinks=True)
    run("/System/Library/Frameworks/CoreServices.framework/Frameworks/LaunchServices.framework/Support/lsregister", "-f", str(destination))
    print(f"Installed: {destination}")
else:
    print(f"Built: {APP}")
