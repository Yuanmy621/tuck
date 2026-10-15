"""Materialize pinned, digest-verified libarchive fixtures without network access."""
import binascii
import hashlib
import json
from pathlib import Path


def prepare_fixtures(root: Path) -> Path:
    folder = root / "Tests/Fixtures/libarchive"
    records = json.loads((folder / "provenance.json").read_text())
    for item in records:
        source = folder / item["source_file"]
        data = source.read_bytes()
        if hashlib.sha256(data).hexdigest() != item["source_sha256"]:
            raise AssertionError(f"Fixture source digest mismatch: {source.name}")
        if "decoded_file" not in item:
            continue
        lines = data.splitlines()
        start = next(i for i, line in enumerate(lines) if line.startswith(b"begin "))
        payload = bytearray()
        for line in lines[start + 1:]:
            if line == b"end":
                break
            payload.extend(binascii.a2b_uu(line))
        decoded = bytes(payload)
        if len(decoded) != item["decoded_bytes"] or hashlib.sha256(decoded).hexdigest() != item["decoded_sha256"]:
            raise AssertionError(f"Decoded fixture digest mismatch: {item['decoded_file']}")
        destination = folder / item["decoded_file"]
        destination.write_bytes(decoded)
    return folder


if __name__ == "__main__":
    print(prepare_fixtures(Path(__file__).resolve().parent.parent))
