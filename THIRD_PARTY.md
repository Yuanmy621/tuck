# Third-party notices

The application links to Apple's system `/usr/lib/libarchive.2.dylib`. It does not bundle Keka or any Keka engine, source, graphic, or other asset.

The two compile-time headers `Sources/ArchiveCore/include/archive.h` and `archive_entry.h` come from the official libarchive v3.8.4 source and retain their BSD license notices:

- https://github.com/libarchive/libarchive/blob/v3.8.4/libarchive/archive.h
- https://github.com/libarchive/libarchive/blob/v3.8.4/libarchive/archive_entry.h

Keka binaries, application icons and engine executables are not included in this repository or the produced app. Research observations and REA records are in `docs/research/`; they are not application source. The original downloaded DMG remains in the original local investigation folder.

The interoperability fixture sources in `Tests/Fixtures/libarchive/` are pinned to libarchive commit `f81f50aa40fc4d94857ca1947da074ee67ea5675`. Companion C files preserve their license notices. Encoded fixture sources and their decoded digests are recorded in `provenance.json`. Tests decode and verify them locally without downloading files. Generated binary archives are ignored.
