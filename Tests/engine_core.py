#!/usr/bin/env python3
"""Production C-boundary archive tests; no Python archive extraction is used."""
import bz2
import ctypes as C
import gzip
import io
import json
import lzma
import os
from pathlib import Path
import shutil
import stat
import subprocess
import sys
import tarfile
import tempfile
import zipfile

ROOT = Path(__file__).resolve().parents[1]
with tempfile.TemporaryDirectory(prefix='tuck-core-') as temporary:
    work = Path(temporary)
    library = work / 'engine.dylib'
    subprocess.run(['clang', '-Wall', '-Wextra', '-Werror', '-dynamiclib', str(ROOT / 'Sources/ArchiveCore/ArchiveCore.c'), '-I', str(ROOT / 'Sources/ArchiveCore/include'), '-larchive.2', '-o', str(library)], check=True)
    lib = C.CDLL(str(library))
    Cancel = C.CFUNCTYPE(C.c_int, C.c_void_p)
    Progress = C.CFUNCTYPE(None, C.c_void_p, C.c_double, C.c_char_p)
    class Callbacks(C.Structure):
        _fields_ = [('context', C.c_void_p), ('is_cancelled', Cancel), ('progress', Progress)]
    lib.ac_create_archive.argtypes = [C.POINTER(C.c_char_p), C.c_size_t, C.c_char_p, C.c_int, C.c_char_p, Callbacks, C.c_char_p, C.c_size_t]
    lib.ac_extract_archive.argtypes = [C.c_char_p, C.c_char_p, C.c_char_p, Callbacks, C.c_char_p, C.c_size_t]
    lib.ac_remove_staging.argtypes = [C.c_char_p, C.c_char_p, C.c_size_t]
    quiet = Callbacks(None, Cancel(lambda _: 0), Progress(lambda *_: None))
    def extract(source, name, password=None, callbacks=quiet, expected=0):
        output = work / name
        output.mkdir()
        error = C.create_string_buffer(4096)
        code = lib.ac_extract_archive(os.fsencode(source), os.fsencode(output), password.encode() if password else None, callbacks, error, len(error))
        assert code == expected, (source, code, expected, error.value)
        return output
    checks = []
    source = work / '资料 空格'
    source.mkdir()
    (source / '空目录').mkdir()
    content = b'\x00\xff' + bytes(range(256)) * 1024
    (source / '说明.txt').write_bytes(content)
    (source / '-script').write_text('#!/bin/sh\necho test\n')
    (source / '-script').chmod(0o751)
    (source / 'link').symlink_to('说明.txt')
    for format, suffix in [(0, '.zip'), (1, '.tar.gz')]:
        archive = work / ('roundtrip' + suffix)
        error = C.create_string_buffer(4096)
        paths = (C.c_char_p * 1)(os.fsencode(source))
        code = lib.ac_create_archive(paths, 1, os.fsencode(archive), format, None, quiet, error, len(error))
        assert code == 0, error.value
        if format == 0:
            with zipfile.ZipFile(archive) as z:
                assert '资料 空格/说明.txt' in z.namelist(), z.namelist()
                assert next(x for x in z.infolist() if x.filename.endswith('说明.txt')).flag_bits & 0x800
        else:
            with tarfile.open(archive) as t:
                assert '资料 空格/说明.txt' in t.getnames(), t.getnames()
        output = extract(archive, 'out-' + str(format))
        assert (output / source.name / '说明.txt').read_bytes() == content
        assert (output / source.name / '空目录').is_dir()
        assert stat.S_IMODE((output / source.name / '-script').stat().st_mode) == 0o751
        assert (output / source.name / 'link').is_symlink()
        assert (output / source.name / 'link').read_bytes() == content
        checks.append('roundtrip-utf8-modes-links-' + suffix)
    for suffix, compress in [('gz', gzip.compress), ('bz2', bz2.compress), ('xz', lzma.compress)]:
        archive = work / ('raw数据.' + suffix)
        archive.write_bytes(compress(content))
        out = extract(archive, 'raw-' + suffix)
        assert (out / 'raw数据').read_bytes() == content
        checks.append('raw-' + suffix)
    plain = work / 'invalid.zip'
    plain.write_bytes(b'This is not an archive\n')
    extract(plain, 'invalid-out', expected=3)
    checks.append('reject-nonarchive')
    for index, name in enumerate(['../escaped', '/tmp/macarchive-escaped', 'a/../../escaped', 'C:/escaped', 'a\\..\\escaped']):
        attack = work / f'traversal{index}.zip'
        with zipfile.ZipFile(attack, 'w') as z: z.writestr(name, b'bad')
        extract(attack, f'traversal-out{index}', expected=3)
    checks.append('reject-five-path-traversals')
    for index, target in enumerate(['../../escape', '/tmp/escape']):
        attack = work / f'link{index}.tar'
        with tarfile.open(attack, 'w') as t:
            entry = tarfile.TarInfo('link')
            entry.type = tarfile.SYMTYPE
            entry.linkname = target
            t.addfile(entry)
        extract(attack, f'link-out{index}', expected=3)
    checks.append('reject-symlink-escapes')
    attack = work / 'symlink-parent.tar'
    with tarfile.open(attack, 'w') as t:
        entry = tarfile.TarInfo('directory')
        entry.type = tarfile.DIRTYPE
        t.addfile(entry)
        entry = tarfile.TarInfo('alias')
        entry.type = tarfile.SYMTYPE
        entry.linkname = 'directory'
        t.addfile(entry)
        entry = tarfile.TarInfo('alias/no-write')
        entry.size = 3
        t.addfile(entry, io.BytesIO(b'bad'))
    out = extract(attack, 'symlink-parent-out', expected=3)
    assert not (out / 'directory/no-write').exists()
    checks.append('reject-write-through-safe-symlink')
    archive = work / 'hardlink.tar'
    with tarfile.open(archive, 'w') as t:
        entry = tarfile.TarInfo('regular')
        entry.size = 4
        t.addfile(entry, io.BytesIO(b'good'))
        entry = tarfile.TarInfo('hardlink')
        entry.type = tarfile.LNKTYPE
        entry.linkname = 'regular'
        t.addfile(entry)
    out = extract(archive, 'hardlink-out')
    assert (out / 'hardlink').read_bytes() == b'good'
    assert (out / 'hardlink').stat().st_ino == (out / 'regular').stat().st_ino
    checks.append('prior-entry-hardlink')
    archive = work / 'duplicate.zip'
    with zipfile.ZipFile(archive, 'w') as z:
        z.writestr('same', b'original')
        z.writestr('same', b'overwrite')
    out = extract(archive, 'duplicate-out', expected=3)
    assert (out / 'same').read_bytes() == b'original'
    checks.append('reject-duplicate-no-overwrite')
    archive = work / 'encrypted.zip'
    subprocess.run(['/usr/bin/zip', '-q', '-P', 'fixture-password', str(archive), '说明.txt'], cwd=source, check=True)
    extract(archive, 'missing-password', expected=1)
    extract(archive, 'wrong-password', password='wrong', expected=1)
    out = extract(archive, 'correct-password', password='fixture-password')
    assert (out / '说明.txt').read_bytes() == content
    checks.append('zipcrypto-missing-wrong-correct-password')
    archive = work / 'cancel.zip'
    with zipfile.ZipFile(archive, 'w', compression=zipfile.ZIP_STORED) as z: z.writestr('big.bin', os.urandom(4 * 1024 * 1024))
    calls = [0]
    def cancel_after_blocks(_):
        calls[0] += 1
        return calls[0] > 15
    cancelling = Callbacks(None, Cancel(cancel_after_blocks), Progress(lambda *_: None))
    extract(archive, 'cancel-out', callbacks=cancelling, expected=2)
    checks.append('midstream-cancellation')
    calls[0] = 0
    big_source = work / 'big-source.bin'
    big_source.write_bytes(os.urandom(4 * 1024 * 1024))
    cancelled_archive = work / 'cancelled-create.zip'
    paths = (C.c_char_p * 1)(os.fsencode(big_source))
    error = C.create_string_buffer(4096)
    code = lib.ac_create_archive(paths, 1, os.fsencode(cancelled_archive), 0, None, cancelling, error, len(error))
    assert code == 2, (code, error.value)
    checks.append('midstream-create-cancellation')
    archive = work / 'duplicate-directory.tar'
    with tarfile.open(archive, 'w') as t:
        for mode in [0o000, 0o751]:
            entry = tarfile.TarInfo('directory')
            entry.type = tarfile.DIRTYPE
            entry.mode = mode
            t.addfile(entry)
    out = extract(archive, 'duplicate-directory-out')
    assert stat.S_IMODE((out / 'directory').stat().st_mode) == 0o751
    checks.append('duplicate-directory-last-mode')
    partial = work / 'restrictive-staging'
    partial.mkdir()
    (partial / 'protected').mkdir()
    (partial / 'protected/data').write_bytes(b'keep')
    (partial / 'protected/link').symlink_to(source)
    (partial / 'protected').chmod(0)
    error = C.create_string_buffer(4096)
    assert lib.ac_remove_staging(os.fsencode(partial), error, len(error)) == 0, error.value
    assert not partial.exists() and source.exists()
    checks.append('cleanup-restrictive-owned-dir-without-following-link')
    print(json.dumps({'ok': True, 'checks': checks, 'count': len(checks)}, ensure_ascii=False))
