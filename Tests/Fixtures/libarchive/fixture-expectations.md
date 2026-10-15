# libarchive 官方测试输入

固定上游 commit 和逐文件 SHA-256 见 provenance.json；保留的 .c 是预期的来源。

- test_read_format_7zip_copy.7z：7-Zip copy 方法；仅普通文件 file1，60 字节，前 4 字节为空格。来源 test_read_format_7zip.c:45–89。
- test_read_format_7zip_lzma1.7z：LZMA1；仅普通文件 file1，2844 字节，前 28 字节为 b"The libarchive distribution "。来源 test_read_format_7zip.c:194–231 的 test_plain_header。
- test_read_format_rar.rar：test.txt 与 testdir/test.txt 都是 b"test text document\r\n" (20 bytes)，另有 symlink testlink -> test.txt、testdir、testemptydir。保守解压器可按策略拒绝链接；不能将这种拒绝说成 RAR 解码不支持。来源 test_read_format_rar.c:47–127。
- test_read_format_rar_subblock.rar：仅普通文件 test.txt，内容 b"test text document\r\n" (20 bytes)，没有前一档案的 symlink。来源 test_read_format_rar.c:130–163。

这些是上游档案，不是 Keka 生成；下载和 uu 解码本身尚未验证应用解码正确。
