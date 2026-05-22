---
name: qt-chat-client
description: "Use for QtChatClient code generation, scaffolding, refactoring, and convention enforcement. 适用于生成 QtChatClient 的界面类、业务逻辑、数据模型、网络交互代码时统一目录结构、命名、职责边界与实现风格。"
argument-hint: "Describe what QtChatClient code to generate and any constraints."
user-invocable: true
---

# QtChatClient Code Generation

## Purpose

这个技能使用来完成QtChatClient项目的功能代码生成任务

## When to Use

<!-- 在这里列出触发场景。 -->

- 新增 Qt 界面类
- 新增客户端业务模块
- 新增数据结构或模型层代码
- 调整文件组织、命名规范、头源文件拆分方式
- 统一信号槽、构造函数、成员变量风格
- 修改QtChatClient具体细节功能

<!-- ## Inputs

在这里说明调用这个技能时，用户应提供哪些信息。

建议填写内容：

- 目标功能 
- 涉及的类名或模块名
- 需要修改或新增的文件
- UI / 网络 / 数据层边界
- 是否需要保持现有 API -->

<!-- ## Project Conventions

在这里写 QtChatClient 的项目约定。

建议填写内容：

- 目录结构约定
- 头文件与源文件位置
- 类命名规则
- 成员变量命名规则
- 信号、槽、私有辅助函数的组织方式
- 是否允许内联实现
- 是否允许新增第三方依赖 -->

## Generation Rules

<!-- 在这里定义生成代码时必须遵守的规则。 -->

1. 生成代码时尽量复用现有逻辑,不重复造轮子
2. 生成代码优先保证代码的可读性和可维护性,而不是追求极致的简洁.
3. 生成代码函数标注批注,说明函数的输入输出,以及函数的核心逻辑和实现思路.且不要过于冗长,保持在合理范围内.
4. 优先遵循现有 QtChatClient 的代码风格。
5. 新增文件时保持头文件与源文件成对出现。
6. 仅修改与需求直接相关的代码，避免顺手重构无关模块。
7. 变更后必须进行最小范围验证。
8. 生成完毕代码后检查QtChatClient CMakeLists.txt是否能正常构建，且相关功能是否按预期工作。

<!-- ## File Layout Rules

在这里明确不同类型代码应放在哪些目录。

可按下面模板补充：

- 界面类：
- 数据模型：
- 公共类型：
- 网络相关：
- 启动与入口： -->

<!-- ## Implementation Checklist

在这里列出生成代码前后的检查项。

可按下面模板补充：

1. 是否找到最接近的现有实现作为参考。
2. 是否确认修改点属于正确的职责层。
3. 是否避免引入无关依赖。
4. 是否补齐必要的头文件与前向声明。
5. 是否验证构建、类型或运行行为。 -->

<!-- ## Output Expectations

在这里定义技能输出应包含什么。

建议填写内容：

- 先说明改动思路
- 再实施最小修改
- 最后报告验证结果
- 如无法完成，明确阻塞点和替代方案 -->

<!-- ## Constraints

在这里写硬性限制。

建议填写内容：

- 不修改哪些目录
- 不引入哪些模式
- 不破坏哪些兼容性要求
- 不覆盖用户已有改动 -->

<!-- ## Examples

在这里放示例请求。

示例占位：

- 生成一个新的聊天主窗口子组件
- 为现有界面补充消息列表模型
- 新增一个网络请求封装类并接入主界面 -->

<!-- ## References

在这里引用后续准备补充的资料文件。

示例占位：

- references/project-structure.md
- references/code-style.md
- assets/ -->
