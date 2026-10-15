# Keka 压缩/解压调查

2026-10-16，研究对象为官网下载的 Keka **1.6.8 (5748)**。本报告基于发布包静态检查、Hopper 伪代码和一次受限 CLI 帮助执行；没有操作 Keka GUI，也没有完成真实 Keka 压缩/解压运行。

## 对象与可信边界

- 官方页面：https://www.keka.io/en/；下载入口 https://d.keka.io，跳转 GitHub `aonez/Keka` 的 v1.6.8 发布资产。原始 HTTP 记录：[download-headers.txt](raw/download-headers.txt)。临时签名下载参数已移除。
- 原始发布包：`Keka.dmg`（未纳入仓库，原始本机副本保留），34,392,393 字节，SHA-256 `25a4daf0492b7e3bca043c8a7ba1ae97507dd8dbf872811daf74b01f16a12125`。
- 主执行文件 SHA-256 `51c730ac9b3d385434fff9e96d0881a458d1fb9513970fffe61b874068085d27`，包含 arm64/x86_64。身份、版本、文档类型见 [REA plist](raw/rea-plist.json)、[Mach-O](raw/rea-macho.json)。
- 签名元数据：`com.aone.keka`，Team `4FG648TM2A`，Developer ID Application: Jorge Garcia Armero；[REA 签名](raw/rea-signature.json)。另执行 `codesign --verify --deep --strict` 返回 0，[记录](raw/codesign-verify.json)。这不等同于联网公证或供应链审计。
- DMG 仅只读挂载并已卸载，未将 Keka 安装/注册为本机应用。CLI 帮助运行触发 macOS 创建 `~/Library/Containers/com.aone.keka`；没有配置其默认文件关联。系统拒绝本进程访问该容器，因此没有删除不明确拥有的容器数据。

## 已证实的结构

**Keka 的界面和任务控制负责组织工作，实际编码/解码分派给不同命令行引擎。**

1. **文件入口。** Info.plist 声明 67 类文档类型，例如 7z、ZIP、RAR、tar、gz 等，角色 Viewer、LSHandlerRank Default；另声明 Finder 服务 `serviceCompress:userData:error:` 和 `serviceExtract:userData:error:`。这是双击/“打开方式”及服务的声明证据，不证明它当前已是默认应用。[REA plist](raw/rea-plist.json)，Evidence `ev_e16a27fcb2589aba9b6743ddd36e75a98e321870fa746bfb285fd897eb5b96b3`。
2. **引擎按格式分工。** 发布包实际包含 `keka7zz`、`keka7z`、`kekaunar`、`kekaunrar`、`kekatar`，以及 pigz/pbzip2/xz/zstd/brotli/lz4/lzip/lrzip 等辅助程序。[完整引擎清单/摘要](raw/engine-inventory.json)；[REA DMG inventory](raw/rea-dmg-inventory.json)，Evidence `ev_a15401b3db0a17444ff28946d42a86e757568fdbdfa101c9f9705026b11a6ba3`。
3. **压缩参数。** `+[compressionController szip:]` (`0x10000bce4`, arm64) 构造 `a`、格式参数，压缩等级、分卷、线程、加密、排除项、目标与输入文件列表，再交给 `setArguments:`。密码存在时添加 `-p`；7z 文件头加密使用 `-mhe`，ZIP AES 使用 `-mem=AES256`，solid 通过 `-ms=on/off`。[伪代码](raw/compression7z.pseudo.c)，Evidence `ev_e9396a6c714f6f224b54043fd0ecf1578e18b377f55b14ddcac1fa44933bc013`。这是反编译解释，不是原始源码。
4. **引擎选择。** `+[compressionController szipBinary:]` 根据全局开关和系统条件选择新引擎（binary 编号 0x16）或 `keka7z`，通过 `NSBundle pathForAuxiliaryExecutable` 得到路径；临时目录传入 `TEMP_PATH_7Z`。[伪代码](raw/engineChoice7z.pseudo.c)，Evidence `ev_b7ff0fbb3e136abce0ee3c534a1c2e9ea7f5d7dee7ea186b9b670cb71acf9736`。静态字符串含 `Use7zz`，但没有通过实际偏好设置验证本机默认选项。
5. **解压分离临时区。** `+[extractionController szip:]` (`0x10001c53c`) 选择新旧引擎，将 cwd 设为 `packageTemporaryPath`，构造 `x`、源文件、`-aou`、`-xr!__MACOSX`、线程参数与进度参数。[伪代码](raw/extraction7z.pseudo.c)，Evidence `ev_c7334ba0c1aac7163798f81919897e314775e328b32a769073a6c038d9c20876`。它能证明先解压到任务临时目录；完整失败回滚、重名策略、链接处理还未逐路径审计。
6. **任务启动与 I/O。** `taskController launchTask:showFormatDetected:` 设置结束观察、管道、计时，再调用 KTask launch；KTask 底层使用 `launchAndReturnError:` / `launch`。[启动伪代码](raw/launchRouting.pseudo.c)、[KTask](raw/taskLaunch.pseudo.c)。`setPipes` 为 stdout/stderr/stdin 设置 NSPipe，并后台读取；tarball 子任务的 stdout 可接后续 stdin。[管道](raw/taskPipes.pseudo.c)，Evidence `ev_09b4f433a4f9b7a3e5eea0ccab525e1992f39d9d0d7b5f8a212acbb135798aaa`。
7. **进度与取消。** 压缩/解压参数含 `-bsp1`；`p7zip progressReader:` 对输出中的百分比进行解析。[进度](raw/7zProgress.pseudo.c)，Evidence `ev_eec0e941acb1b8f2f7123ec1805f6bcfe21bba8f6e6326df7e1bd22aee94af8b`。`Globals forceTaskToQuit:` 检查 running 后调用 terminate 并排队后续工作。[取消](raw/cancelTask.pseudo.c)，Evidence `ev_26ff6293fc49f9e446cb9e6196bd12e4981d638dc3c54ef322780dbe05898863`。没有验证取消后所有中间文件必然清理。
8. **密码不是只有命令行明文参数。** 压缩构造器只添加裸 `-p`；另有 `KTask getPasswordFile:`，把密码写到容器临时目录下的 UUID 路径，KTask launch 随后调用 `removePasswordFileAlmostImmediatly`。[密码文件](raw/passwordFile.pseudo.c)、[KTask 启动](raw/taskLaunch.pseudo.c)。密码文件如何在所有引擎间传输、权限和清理竞态未全面追踪，不据此声称它的秘密管理绝对安全。

## 实际执行与限制

- 直接执行包内 `keka7zz --help` 得到 signal 5，无输出；REA retained capture：[rea-engine-help.json](raw/rea-engine-help.json)，Evidence `ev_987ce253d64f98f82a3256658ffdc8f30ca7206f90fc4f584c367c01d6859e9f`。辅助程序声明 sandbox + inherit，不能把这个失败称为引擎本身损坏。
- 通过发布包提供的 CLI 入口 `Keka --ignore-file-access --cli 7zz --help` 成功返回 0：[stdout](raw/keka-cli-ignore-help.stdout.txt)。实际自报 **7-Zip 26.03，Modified by aone for Keka**；包内 [About.html](raw/en.lproj-About.html) 仍写 26.02，因此报告以这两类证据分别记录，不擅自抹平差异。keka7zz SHA-256 `2885552cce94f80fd95fac93eb5ecc4fdb19f3f7b7fa8180930bb71cbb0387d2`。
- 对自建 `reference-input/说明 文本.txt` 的 ZIP/7z/密码 ZIP 压缩尝试被 macOS App Sandbox 拒绝，exit 2，**未生成 Keka 参考压缩包**。[全部指令/执行文件摘要/退出码](reference-fixtures/creation-log.json)，[stderr 示例](reference-fixtures/keka-reference.zip.stderr.txt)。没有改签、扩大文件授权或声称 GUI 动态复现。

## 如何用于独立实现

这些证据支持采用“原生窗口 + 文档打开事件 + 任务控制 + 现成归档库”的架构。双击压缩包可由自家 Info.plist 声明类型并接收系统打开事件实现；系统默认应用选择是用户配置。采用哪个库、支持哪些格式、临时输出目录、取消和碰撞策略，属于本次实现的独立设计选择，不能当作已经完整复制了 Keka。

Keka 本体、图标和辅助二进制没有纳入本仓库或交付应用。相关反编译伪代码仅作为调研证据保存在 `raw/`，不参与编译。功能验收应使用可复现的自建档案和上游互操作档案，逐项确认文件内容、Unicode 路径、损坏/加密输入、重名和路径边界，不能用“库支持格式”代替实际验证。

## 互操作样本

[互操作样本](../../Tests/Fixtures/libarchive/) 来自 libarchive 官方仓库固定 commit `f81f50aa40fc4d94857ca1947da074ee67ea5675`。原 `.uu`、解码后档案、上游 `.c` 检查及 SHA-256 保留在 [provenance.json](../../Tests/Fixtures/libarchive/provenance.json)。**它们是 libarchive 上游测试输入，不是 Keka 生成的档案。** 预期见 [fixture-expectations.md](../../Tests/Fixtures/libarchive/fixture-expectations.md)。测试是否在本机通过应由独立实现的测试记录回答，本调查不预先宣称通过。

所有 REA Evidence ID 和所对应完整 JSON：见 [evidence-index.json](raw/evidence-index.json)。
