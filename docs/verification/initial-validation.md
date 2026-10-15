# 原始版本验证记录

> 本文记录更名前的原始验收，名称、绝对路径和可执行文件摘要保留其历史含义。Tuck 的重新构建及测试见 [当前验证记录](current-validation.md)。

日期：2026-10-16。主机：Apple Silicon，macOS 27.0.1。REA CLI 6.5.0；Swift 6.4，Swift 5 语言模式。

交付应用：`/Users/ymy/Applications/轻压.app`，bundle ID `local.ymy.MacArchive`。最终可执行文件 SHA-256 见 [finder-validation.json](finder-validation.json) 的 `final_executable_sha256`。

## 原始任务对应结果

| 请求 | 结果与边界 |
| --- | --- |
| 弄清 Keka 压缩/解压如何工作 | 已通过官方发布包、REA、Hopper 伪代码确认文件入口、参数构造、引擎选择、临时输出、管道、进度与取消；Keka GUI 实际操作未复现，清理和密码传输未全面审计 |
| 展示证据 | 完整原始 REA JSON、伪代码、命令与身份摘要保存在本目录，入口为 [Keka调查.md](../research/keka.md) |
| 实现 Mac 原生小应用 | 已实现、构建、临时签名、安装并启动；源代码位于上级目录 |
| 双击压缩包解压 | 最终安装包已从未运行状态，通过 Finder 实际双击 ZIP 启动并解压，内容验证通过；已有结果保留，第二次生成 `双击解压示例 (2)` |
| 压缩文件 | 原生选择文件、保存对话框、创建 ZIP 已实际执行，独立 Python ZIP 读取器核对内容一致；拖入文件入口已实现，但本轮没有执行物理跨窗口拖拽操作 |

## 应用入口黑盒测试

命令：

```sh
python3 Tests/integration.py '/Users/ymy/Applications/轻压.app/Contents/MacOS/MacArchive'
```

**24 项全部通过**，机器记录：[validation.json](validation.json)。

覆盖：ZIP / TAR.GZ 独立读取器与字节一致性、中文/空格/前导横线/二进制/空目录；重名解压不覆盖；压缩目标已存在不覆盖；Python 生成 ZIP；单文件 gz/gzip/bz/bz2/xz；绝对路径、`..`、Windows 驱动器与反斜杠路径；逃逸软链接；合法相对软链接；损坏输入；预先取消；非目录目标；ZipCrypto 的无密码、错误密码及正确密码；上游 7z Copy、7z LZMA、RAR 普通文件与含安全软链接样本。

7z/RAR 输入来自固定 libarchive 上游提交，来源与摘要见 [interop-fixtures/provenance.json](../../Tests/Fixtures/libarchive/provenance.json)。这些不是 Keka 运行产生的档案。

## 引擎生产边界测试

命令：`python3 Tests/engine_core.py`。

C 代码使用 `-Wall -Wextra -Werror` 编译，**16 组全部通过**，机器记录：[engine-validation.json](engine-validation.json)。

补充覆盖：UTF-8 ZIP 标志、文件执行权限、安全链接、已有普通文件硬链接、通过软链接目录写入拒绝、重复文件拒绝、流式解压取消、流式压缩取消、重复目录权限以最后条目为准、权限为 000 的目录清理及清理不跟随链接。

## Finder、界面与默认打开方式

- 通过 CUA 实际双击 Finder 的 `Examples/双击解压示例.zip`，应用显示“解压完成”；已独立读取输出内容并确认原包保留。
- 通过 CUA 操作原生文件选择与保存窗口，创建 `Examples/双击解压示例/界面压缩测试.zip`，独立 ZIP 读取器验证成功。
- 原始默认工具保存在首次备份中。实际点击“恢复原默认工具”，接受 macOS 逐类型确认后，独立查询 10 类类型的默认工具，全部与原始记录一致。随后重新设置轻压，接受系统确认，核对全部关联指向轻压，且首次备份摘要不变。
- 默认关联最初使用的旧接口曾返回成功却未改变实际关联；最终代码使用 `NSWorkspace.setDefaultApplication` 的异步完成语义，并核对实际处理应用后才显示成功。
- UI 已截图检查，修复了重复的格式标签。
- 最终应用已通过 `codesign --verify --deep --strict`。这是本地临时签名验证，不是开发者公证。

记录：[finder-validation.json](finder-validation.json)、[association-validation.json](association-validation.json)、[associations-before.json](associations-before.json)、[associations-after.json](associations-after.json)、[final-build.log](initial-build.log)。

## 未验证或当前不包含

- Keka GUI 的真实压缩/解压：发布包 CLI 的样本写入被其沙盒拒绝，未绕过保护，未声称完成动态对照。
- 加密压缩创建、7z 创建、分卷和 Finder 右键服务。
- AES ZIP、加密 7z/RAR、所有 RAR/7z 编码变体和多卷输入。
- 全部扩展属性、resource fork、ACL、Finder 标签及时间戳的完整往返。
- 当前主机之外的 macOS 版本和 Intel 主机实际运行。
- 没有生产性能基准、CPU/峰值内存测量或压缩率对照；测试耗时不是性能承诺。

实现采用 macOS 系统 libarchive 和独立 Swift/C 代码，没有复制 Keka 的图标、应用代码或引擎到交付应用。REA 仓库保持未修改。
