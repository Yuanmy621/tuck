#!/usr/bin/env python3
"""Black-box archive interchange, filesystem confinement and cleanup checks."""
import bz2
import gzip
import hashlib
import io
import json
import lzma
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tarfile
import tempfile
import time
import zipfile
from fixtures import prepare_fixtures

ROOT = Path(__file__).resolve().parent.parent
INTEROP = prepare_fixtures(ROOT)
EXE = Path(sys.argv[1]) if len(sys.argv) > 1 else ROOT / "build/Tuck.app/Contents/MacOS/Tuck"
checks = []
started = time.monotonic()


def invoke(*args, success=True):
    p = subprocess.run([str(EXE), "--cli", *map(str, args)], capture_output=True, text=True, timeout=45)
    try:
        result = json.loads(p.stdout)
    except Exception as e:
        raise AssertionError(f"Invalid JSON: {args}: {p.returncode}, {p.stdout}, {p.stderr}") from e
    assert bool(result["ok"]) == success, (args, result, p.stderr)
    assert (p.returncode == 0) == success, (args, result, p.returncode)
    return result


def check(name, operation):
    operation()
    checks.append({"name": name, "status": "passed"})
    print(f"PASS {name}", flush=True)


with tempfile.TemporaryDirectory(prefix="Tuck-integration-") as temporary:
    base = Path(temporary)
    inputs = base / "资料 空格"
    inputs.mkdir()
    (inputs / "空目录").mkdir()
    (inputs / "说明.txt").write_text("你好，Tuck。\nUnicode — café 🍊\n", encoding="utf8")
    binary = bytes(range(256)) * 1024
    (inputs / "binary.dat").write_bytes(binary)
    (inputs / "--leading-dash.txt").write_text("not a command", encoding="utf8")
    output = base / "results"
    output.mkdir()

    def validate_tree(folder):
        original = folder / inputs.name
        assert (original / "说明.txt").read_bytes() == (inputs / "说明.txt").read_bytes()
        assert (original / "binary.dat").read_bytes() == binary
        assert (original / "--leading-dash.txt").read_text() == "not a command"
        assert (original / "空目录").is_dir()

    archives = {}
    for format, extension in [("zip", "zip"), ("tarGzip", "tar.gz")]:
        archive = output / f"roundtrip-{format}.{extension}"
        archives[format] = archive

        def roundtrip(format=format, archive=archive):
            invoke("compress", format, archive, inputs)
            if format == "zip":
                with zipfile.ZipFile(archive) as z:
                    assert z.testzip() is None
                    assert z.read(f"{inputs.name}/binary.dat") == binary
            else:
                with tarfile.open(archive) as t:
                    assert t.extractfile(f"{inputs.name}/binary.dat").read() == binary
            result = invoke("extract", archive, output)
            validate_tree(Path(result["output"]))
        check(f"{format}: independent-reader and byte-exact roundtrip", roundtrip)

    def collisions():
        archive = archives["zip"]
        first = Path(invoke("extract", archive, output)["output"])
        marker = first / "keep.txt"
        marker.write_text("keep")
        second = Path(invoke("extract", archive, output)["output"])
        assert first != second and marker.read_text() == "keep"
        validate_tree(second)
    check("extraction collision preserves existing result", collisions)

    def refuse_overwrite():
        archive = archives["zip"]
        digest = hashlib.sha256(archive.read_bytes()).digest()
        invoke("compress", "zip", archive, inputs, success=False)
        assert hashlib.sha256(archive.read_bytes()).digest() == digest
    check("compression never overwrites existing destination", refuse_overwrite)

    def independent_zip():
        archive = output / "python-created.zip"
        with zipfile.ZipFile(archive, "w", zipfile.ZIP_DEFLATED) as z:
            z.writestr("Windows 文件夹/你好.txt", "跨平台 ZIP\n")
        folder = Path(invoke("extract", archive, output)["output"])
        assert (folder / "Windows 文件夹/你好.txt").read_text() == "跨平台 ZIP\n"
    check("ZIP produced by Python extracts correctly", independent_zip)

    for extension, encoder in [("gz", gzip.compress), ("gzip", gzip.compress), ("bz", bz2.compress), ("bz2", bz2.compress), ("xz", lzma.compress)]:
        def raw_stream(extension=extension, encoder=encoder):
            archive = output / f"plain.txt.{extension}"
            data = "单文件压缩内容\n".encode()
            archive.write_bytes(encoder(data))
            folder = Path(invoke("extract", archive, output)["output"])
            assert (folder / "plain.txt").read_bytes() == data, list(folder.iterdir())
        check(f"standalone {extension} stream keeps original filename", raw_stream)

    for label, name in [("parent-traversal", "../outside.txt"), ("absolute-path", str(base / "outside.txt")), ("windows-drive", "C:/outside.txt"), ("windows-traversal", "..\\outside.txt")]:
        def attack(label=label, name=name):
            archive = output / f"{label}.zip"
            with zipfile.ZipFile(archive, "w") as z:
                z.writestr("valid.txt", "staged")
                z.writestr(name, "must not escape")
            before = set(output.iterdir())
            invoke("extract", archive, output, success=False)
            assert not (base / "outside.txt").exists()
            assert set(output.iterdir()) == before, "failed extraction left partial output"
        check(f"reject {label}; no partial output", attack)

    def symlink_attack():
        archive = output / "symlink-escape.tar"
        with tarfile.open(archive, "w") as t:
            e = tarfile.TarInfo("link")
            e.type = tarfile.SYMTYPE
            e.linkname = "../"
            t.addfile(e)
            f = tarfile.TarInfo("link/outside.txt")
            f.size = 6
            t.addfile(f, io.BytesIO(b"escape"))
        before = set(output.iterdir())
        invoke("extract", archive, output, success=False)
        assert not (output / "outside.txt").exists()
        assert set(output.iterdir()) == before
    check("reject escaping symlink and preserve boundary", symlink_attack)

    def valid_symlink():
        archive = output / "safe-link.tar"
        with tarfile.open(archive, "w") as t:
            f = tarfile.TarInfo("target.txt")
            f.size = 4
            t.addfile(f, io.BytesIO(b"safe"))
            e = tarfile.TarInfo("alias.txt")
            e.type = tarfile.SYMTYPE
            e.linkname = "target.txt"
            t.addfile(e)
        folder = Path(invoke("extract", archive, output)["output"])
        assert (folder / "alias.txt").is_symlink()
        assert (folder / "alias.txt").read_bytes() == b"safe"
    check("safe relative symlink roundtrip", valid_symlink)

    def malformed():
        archive = output / "broken.zip"
        archive.write_bytes(b"this is not a zip")
        before = set(output.iterdir())
        invoke("extract", archive, output, success=False)
        assert set(output.iterdir()) == before
    check("malformed input fails without output residue", malformed)

    def cancelled():
        before = set(output.iterdir())
        result = invoke("cancel-extract", archives["zip"], output, success=False)
        assert result["kind"] == "cancelled", result
        assert set(output.iterdir()) == before
    check("cancelled operation creates no output", cancelled)

    def invalid_parent():
        invoke("extract", archives["zip"], inputs / "说明.txt", success=False)
    check("non-directory extraction destination rejected", invalid_parent)

    def password_zip():
        plain = base / "password.txt"
        plain.write_bytes(b"password protected fixture\n")
        archive = output / "password.zip"
        subprocess.run(["/usr/bin/zip", "-q", "-P", "fixture-only-password", str(archive), plain.name], cwd=base, check=True)
        missing = invoke("extract", archive, output, success=False)
        wrong = invoke("extract", archive, output, "--password", "wrong", success=False)
        assert missing["kind"] == "passwordRequired", missing
        assert wrong["kind"] == "passwordRequired", wrong
        result = invoke("extract", archive, output, "--password", "fixture-only-password")
        assert (Path(result["output"]) / plain.name).read_bytes() == plain.read_bytes()
    check("encrypted ZIP: missing, wrong and correct passwords", password_zip)

    interop = INTEROP
    for name, member, size, prefix in [
        ("test_read_format_7zip_copy.7z", "file1", 60, b"    "),
        ("test_read_format_7zip_lzma1.7z", "file1", 2844, b"The libarchive distribution "),
        ("test_read_format_rar_subblock.rar", "test.txt", 20, b"test text document\r\n"),
        ("test_read_format_rar.rar", "test.txt", 20, b"test text document\r\n"),
    ]:
        archive = interop / name
        if not archive.exists():
            raise AssertionError(f"Required interoperability fixture missing: {archive}")
        def upstream_fixture(archive=archive, member=member, size=size, prefix=prefix):
            folder = Path(invoke("extract", archive, output)["output"])
            data = (folder / member).read_bytes()
            assert len(data) == size and data.startswith(prefix), (archive, len(data), data[:32])
            if archive.name == "test_read_format_rar.rar":
                assert (folder / "testlink").is_symlink()
                assert (folder / "testlink").read_bytes() == data
                assert (folder / "testemptydir").is_dir()
        check(f"official libarchive interoperability: {name}", upstream_fixture)

report = {"checks": checks, "count": len(checks), "elapsed_seconds": round(time.monotonic() - started, 3), "executable": str(EXE)}
report_path = ROOT / ".local/verification/integration.json"
report_path.parent.mkdir(parents=True, exist_ok=True)
report_path.write_text(json.dumps(report, ensure_ascii=False, indent=2) + "\n")
print(json.dumps({"passed": len(checks), "elapsed_seconds": report["elapsed_seconds"], "report": str(report_path)}, ensure_ascii=False))
