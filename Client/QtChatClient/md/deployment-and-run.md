# QtChatClient 部署与运行

本文档描述 QtChatClient 当前的构建依赖、部署方式、运行步骤与已确认的运行结果。

## 1. 构建基础

项目当前使用 CMake + Qt6 Widgets 构建。

从 CMakeLists.txt 可以确认：

1. 最低 CMake 版本要求为 3.19。
2. 项目语言为 C++。
3. 依赖 Qt6 的 Core 和 Widgets 组件。
4. 资源文件通过 resources/resource.qrc 打包进可执行程序。
5. 项目会递归收集 include 和 src 下的头文件、源文件。

## 2. 环境依赖

当前本地环境下，QtChatClient 在 Ubuntu 24.04 的已确认依赖包括：

1. Qt6 开发环境。
2. CMake。
3. 支持 Qt6Gui 和 Qt6Widgets 的 OpenGL 开发包。

如果 Linux 环境缺少 OpenGL 开发依赖，可能出现 WrapOpenGL not found 一类错误。当前已确认可通过以下命令安装：

```bash
sudo apt install libgl1-mesa-dev libopengl-dev libglx-dev
```

## 3. Qt 路径发现策略

当前 CMakeLists 已内置 Qt 安装路径自动发现逻辑。若未显式提供 Qt6_DIR 或 CMAKE_PREFIX_PATH，构建脚本会尝试扫描以下目录：

1. ~/Qt/\*/gcc_64
2. ~/Qt6/\*/gcc_64

这意味着在 Linux 本地通过 Qt Online Installer 安装 Qt 时，通常可以直接运行 CMake，而不必手工再传一次 CMAKE_PREFIX_PATH。只有 Qt 安装在非标准位置时，才需要手动指定。

## 4. 构建步骤

当前推荐的基本构建流程如下：

```bash
cmake -S . -B build
cmake --build build
```

如果 Qt 未被自动发现，可补充传入安装前缀：

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64
cmake --build build
```

## 5. 安装与部署

项目当前已在 CMakeLists 中声明 install 规则，并通过 qt_generate_deploy_app_script 生成部署脚本。这说明项目已经具备基础的安装与部署脚本支持。

常规安装步骤可使用：

```bash
cmake --install build
```

当前文档仅说明构建系统已具备安装/部署入口；不同平台上的最终发布目录结构，后续可在部署流程稳定后单独补充。

## 6. 运行步骤

完成构建后，可直接运行 build 目录下的程序：

```bash
./build/QtChatClient
```

如果当前工作目录已经位于 build，也可直接运行：

```bash
./QtChatClient
```

## 7. 当前运行结果

根据当前代码实现与本地已执行结果，现阶段已确认：

1. 程序可以成功启动，最近一次本地执行 ./QtChatClient 返回码为 0。
2. 启动后会显示主窗口，窗口标题为 Qt Chat Client。
3. 窗口图标来自 :/images/logo.png。
4. 主界面为左中右三栏布局。
5. 左侧导航默认激活“会话”标签。
6. 点击“会话 / 好友 / 好友申请”按钮时，会切换对应图标状态。

## 8. 当前运行阶段说明

虽然程序已能正常启动，但当前仍处于“界面骨架可运行”阶段，主要限制如下：

1. 中间列表区尚未填充业务内容。
2. 右侧主内容区尚未填充聊天详情内容。
3. 消息附件 ID 仍为占位值。
4. 还未接入完整的聊天网络流程和业务数据加载流程。
