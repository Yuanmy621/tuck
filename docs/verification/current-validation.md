# Tuck 构建与整理验证

日期：2026-10-16。主机：Apple Silicon / macOS 27.0.1；Swift 6.4、Swift 5 语言模式。

## 改名与应用

- 品牌、窗口、菜单、Swift target、可执行文件和应用包已统一为 **Tuck**。
- 已在当前仓库目录重新构建、临时签名、校验并安装到 `~/Applications/Tuck.app`。
- 原“轻压.app”按身份校验后备份到 `~/Applications/.Tuck-backups/`。
- 稳定 bundle ID `local.ymy.MacArchive` 及首次默认工具备份保留。备份摘要与原验收一致。
- 系统对 RAR 和 7z 再次要求确认，已通过原生设置入口确认；独立查询全部 10 类关联均指向 Tuck 的稳定身份。
- 实际通过 Finder 双击 `Tuck Demo.zip`，Tuck 窗口显示解压完成；独立读取输出文件确认中英文内容一致。UI 显示名称为 Tuck。

记录：[tuck-rename.json](tuck-rename.json)、[tuck-build.log](tuck-build.log)。

## 执行的测试

| 命令 | 结果 |
| --- | --- |
| `python3 scripts/build-app.py` | 发布构建与临时签名校验通过 |
| `python3 Tests/integration.py` | 24 项应用黑盒检查通过 |
| `python3 Tests/engine_core.py` | 16 组 C 引擎边界检查通过；`-Wall -Wextra -Werror` 编译通过 |
| 安装后原生窗口与 Finder 双击 | Tuck 名称正确，ZIP 自动解压，输出内容一致 |
| 调研记录与链接校验 | 22 份保留的原始 JSON 均可解析，且与原始副本逐字节一致；文档相对链接可解析 |

机器记录：[tuck-integration.json](tuck-integration.json)、[tuck-engine.json](tuck-engine.json)。

互操作输入已移到 `Tests/Fixtures/libarchive/`。测试从固定 `.uu` 源码解码，核对源文件及解码结果摘要；这些检查为必需项，不再因目录不存在而跳过。生成档案和新测试输出均写入 Git 忽略位置，不改写历史调研记录。

## 整理边界

- 本仓库包含独立实现源码、图标生成代码、测试源、固定上游样本来源、Keka 调研和支持报告的原始记录。
- Keka DMG、Keka 可执行文件、辅助引擎、编译产物、应用包、缓存、生成的测试档案、无关整份资源/字符串转储不进入 Git。
- 原始本机项目保留，不改写其 JSON、旧名字、路径、版本、摘要或 Evidence ID。更名前的验收单独标注为[历史记录](initial-validation.md)。

没有重新执行 Keka GUI 调查，也没有扩大旧报告的结论。AES 加密、分卷、所有 RAR/7z 变体、完整扩展属性保留和其他主机运行仍未验证。拖拽入口已实现，本轮原生手动验证使用 Finder 双击；没有执行物理跨窗口拖拽。未运行生产性能基准。
