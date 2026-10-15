# Tuck

A small native macOS archiver. Drop files to pack them, double-click an archive to unpack it.

**Tuck** 取“收好、放妥”的意思，是这个项目的英文名称。界面保留中文操作提示。

## 日常使用

- **解压**：在 Finder 中双击关联的压缩包。Tuck 在原包旁边创建独立文件夹，保留原包；遇到同名结果自动使用 `名称 (2)` 等新目录。
- **压缩**：打开 Tuck，把文件或文件夹拖入窗口，选择 ZIP 或 TAR.GZ，点击“压缩并保存…”。也可以使用原生文件选择器。
- **批量与取消**：多个压缩包依次处理，支持取消；处理中退出会等待任务清理。
- **密码**：支持的加密 ZIP 会提示输入本次解压密码，不保存密码。
- **默认工具**：“设为默认解压工具”和“恢复原默认工具”使用 macOS 的系统确认流程。首次备份不会被再次设置覆盖。

| 操作 | 当前范围 |
| --- | --- |
| 创建 | ZIP（Deflate）、TAR.GZ |
| 解压 | ZIP、TAR、TAR.GZ/TGZ、TAR.BZ2/TBZ/TBZ2、TAR.XZ/TXZ、7z、RAR、单文件 Gzip/Bzip2/XZ |
| 已验证的加密输入 | ZipCrypto ZIP：无密码、错误密码、正确密码 |
| 尚未实现或验证 | 创建 7z、加密压缩、分卷、Finder 右键服务；AES ZIP 与加密 7z/RAR 未验证 |

格式支持受 macOS 系统 libarchive 的具体编码能力限制。扩展属性、resource fork、ACL、Finder 标签与所有时间戳不保证完整往返。解压拒绝绝对路径、`..`、经过软链接目录写入，以及部分保守策略不接受的链接；不会覆盖已有文件。

## 构建

需要 macOS 14+、Xcode 或 Command Line Tools，以及 Python 3。当前交付为 Apple Silicon；其他主机的实际运行范围见验证记录。使用系统 libarchive，不需要 Homebrew 或额外压缩引擎。

在本仓库目录运行：

```sh
python3 scripts/build-app.py
```

生成 `build/Tuck.app`，正常使用不需要终端。安装到当前用户 Applications：

```sh
python3 scripts/build-app.py --install
```

安装前退出正在运行的 Tuck 或原“轻压”。同身份的已有应用会备份到 `~/Applications/.Tuck-backups/`，原“轻压.app”也会移入该备份目录。安装不自动改变默认打开方式。

为保留已有文件关联和首次恢复记录，应用继续使用稳定 bundle ID `local.ymy.MacArchive`，备份仍位于 `~/Library/Application Support/MacArchive/previous-defaults.json`。名称、可执行文件与 Swift target 已统一为 Tuck；这些持久身份不随品牌名称改变。

本地构建使用临时签名，未做开发者公证。

## 验证

```sh
python3 Tests/integration.py
python3 Tests/engine_core.py
```

互操作测试从仓库内固定版本的 `.uu` 源文件解码 7z/RAR 输入，并核对来源与解码摘要，无需网络。缺少或篡改样本会使测试失败。生成的二进制样本和本机测试输出不进入 Git。

- [当前构建与验证](docs/verification/current-validation.md)
- [更名前的原始验收](docs/verification/initial-validation.md)

## 源码与调研

| 路径 | 内容 |
| --- | --- |
| `Sources/Tuck/` | SwiftUI 窗口、文件打开事件、任务队列、密码与默认关联 |
| `Sources/ArchiveCore/` | C 流式归档引擎、路径和链接边界、取消与清理 |
| `scripts/` | 应用打包、安装与代码绘制图标 |
| `Tests/` | 应用黑盒、C 边界测试及固定互操作输入 |
| `docs/research/` | Keka 调研报告、REA 记录、相关反编译伪代码与失败日志 |
| `docs/verification/` | 历史验收与改名后的验证记录 |

[Keka 调研报告](docs/research/keka.md) 区分了发布包事实、反编译观察、实际执行与未知。Keka 的原生任务层组织参数，调用 7-Zip、tar、unar 等引擎并读取管道进度；Tuck 独立实现界面和任务层，使用 macOS 系统 libarchive。

仓库不包含 Keka 应用、DMG、图标或辅助引擎。研究证据不参与应用编译。原始 REA JSON 的 Evidence ID、源路径和摘要保留历史含义，未因目录整理或应用改名而修改。

第三方来源与许可证见 [THIRD_PARTY.md](THIRD_PARTY.md)。
