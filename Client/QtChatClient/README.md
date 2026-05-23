# QtChatClient

QtChatClient 的说明文档已拆分为“总览索引 + 主题文档”结构。

## 文档导航

| 文档                                                 | 作用                                             |
| ---------------------------------------------------- | ------------------------------------------------ |
| [md/data-definition.md](md/data-definition.md)       | 说明项目核心数据结构、消息构造规则与基础公共能力 |
| [md/feature-overview.md](md/feature-overview.md)     | 说明当前已实现的界面结构、功能点与模块职责       |
| [md/architecture.md](md/architecture.md)             | 说明项目模块划分、依赖关系、职责边界与演进方向   |
| [md/protocol-and-api.md](md/protocol-and-api.md)     | 说明客户端与服务端协议、接口约定与消息流转设计   |
| [md/ui-design.md](md/ui-design.md)                   | 说明页面结构、交互流程、状态切换与界面设计约束   |
| [md/development-plan.md](md/development-plan.md)     | 说明当前阶段目标、开发里程碑、待办项与推进计划   |
| [md/deployment-and-run.md](md/deployment-and-run.md) | 说明构建依赖、部署方式、运行步骤与当前运行结果   |
| [md/testing.md](md/testing.md)                       | 说明当前测试现状、手工验证项与后续测试补充方向   |

## 当前项目概览

QtChatClient 是一个基于 Qt6 Widgets 的聊天客户端窗口程序，当前已完成：

1. 基础数据模型定义。
2. 公共工具与日志能力封装。
3. 主窗口三栏骨架布局。
4. 左侧导航标签切换逻辑。

## 文档维护约定

后续更新文档时，按以下方式维护：

1. 根 README.md 只负责目录、导航和整体概览。
2. 数据定义相关内容统一更新到 md/data-definition.md。
3. 功能与界面行为相关内容统一更新到 md/feature-overview.md。
4. 架构设计、模块职责、依赖边界相关内容统一更新到 md/architecture.md。
5. 客户端与服务端协议、接口约定相关内容统一更新到 md/protocol-and-api.md。
6. 页面结构、交互设计、视觉与状态流转相关内容统一更新到 md/ui-design.md。
7. 开发阶段目标、里程碑、待办计划相关内容统一更新到 md/development-plan.md。
8. 部署、构建、运行结果相关内容统一更新到 md/deployment-and-run.md。
9. 测试、验证、测试计划相关内容统一更新到 md/testing.md。

如果后续文档规模继续增长，再在 md 目录下按主题继续细分，但根 README 仍保持索引角色。
