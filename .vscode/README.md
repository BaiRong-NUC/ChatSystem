# VS Code Workspace Notes

这个目录保存 ChatSystem 工作区在 VS Code 下使用的本地配置。

## 当前用途

- c_cpp_properties.json 为 C/C++ 扩展提供 Qt6 的 IntelliSense 配置。
- 当前配置用于解决 Qt 头文件无法解析的问题，例如 QApplication、QWidget 和 Q_OBJECT 宏展开时报错。
- IntelliSense 当前使用标准 CMake 导出的 compile_commands.json，而不是 Qt Creator 自己生成的 .qtc_clangd 数据库。

## 当前关键路径

- Qt 安装目录：/home/bairong/Qt6/6.9.3/gcc_64
- 编译数据库：${workspaceFolder}/Client/QtChatClient/build/cmake-verify/compile_commands.json
- 编译器：/usr/bin/g++

## 何时需要修改配置

以下情况需要同步更新 c_cpp_properties.json：

- Qt 安装路径变了
- 使用了新的构建目录
- compile_commands.json 被重新生成到别的位置
- 切换了编译器或工具链

## 推荐做法

如果 IntelliSense 再次出现无法打开 Qt 头文件的问题，按这个顺序检查：

1. 确认 QtChatClient 可以被 CMake 正常配置和构建。
2. 确认 build/cmake-verify/compile_commands.json 仍然存在。
3. 确认 c_cpp_properties.json 里的 Qt include 路径仍然有效。
4. 在 VS Code 中重置 IntelliSense 数据库，或直接重载窗口。

## 说明

这个目录只影响 VS Code 的编辑体验，不影响项目实际的 CMake 构建结果。
