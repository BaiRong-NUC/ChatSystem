# 内存管理与泄漏检测

## 所有权约定

本项目同时使用标准 C++ RAII 和 Qt 对象树：

- 本约定同时受仓库根目录 `AGENTS.md` 约束，并适用于后续 C++ 后端和 Qt 代码。
- 能明确所有权时优先使用智能指针，不新增负责所有权的裸指针。
- 顶层窗口以及普通 C++ 数据使用 `std::unique_ptr`，明确表达唯一所有权。
- 带有 `QObject` 父对象的控件由 Qt 父子对象树释放；类成员使用 `QPointer` 观察对象，目标销毁后会自动变为 `nullptr`。
- 动态创建后交给 Qt 布局或容器的对象，先由 `std::unique_ptr` 临时持有，成功加入后调用 `release()` 转移所有权。
- 非拥有型临时指针，例如 `layout()`、`widget()`、`verticalScrollBar()` 的返回值，保持裸指针，不负责释放。

不要用 `std::shared_ptr` 管理由 Qt 父对象拥有的 `QObject`，否则两个所有者可能重复销毁同一对象。

## 本次发现并修复的风险

1. `MessageWidget::AddMessage` 和 `AddFrontMessage` 曾使用 `new QFont(...)` 作为默认参数。每次省略参数都会产生无人释放的分配，现已改为 `const QFont &` 和值类型默认值。
2. `MainWidget`、左右栏及其子区域曾使用静态裸指针加 `new` 实现单例。它们没有释放路径，现已删除这些单例，顶层窗口由 `std::unique_ptr` 管理，子控件由 Qt 对象树管理。
3. `MessageItem` 曾创建裸指针消息副本，再由 `ChatMessage` 手工 `delete`。现由 `std::unique_ptr<Model::Message>` 明确拥有消息副本。
4. 多个 QWidget 析构函数手工删除已经有父对象的子控件。虽然 Qt 通常能从父对象列表中移除已删除子对象，但这种双重所有权表达容易产生重复释放和悬空访问，现已交还 Qt 对象树统一处理。
5. 清空消息和会话列表时曾手工管理 `QLayoutItem`/`QWidget`，或依赖 `deleteLater()` 延后释放。现使用局部 `std::unique_ptr`，操作结束时立即释放。

## 复测命令

常规构建：

```bash
cmake --build build -j2
```

使用 AddressSanitizer、LeakSanitizer 和 UndefinedBehaviorSanitizer：

```bash
cmake -S Client/QtChatClient -B /tmp/chatsystem-asan-build \
  -DCMAKE_BUILD_TYPE=Debug \
  -DQTCHATCLIENT_ENABLE_SANITIZERS=ON
cmake --build /tmp/chatsystem-asan-build -j2
QT_QPA_PLATFORM=offscreen \
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 \
UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 \
/tmp/chatsystem-asan-build/QtChatClient --smoke-test
```

`--smoke-test` 会创建完整主界面和内置测试数据，然后通过正常事件循环退出，使所有析构函数得到执行。命令退出码为 0 且没有 Sanitizer 报告，表示该启动/退出路径没有检测到泄漏、越界访问、释放后使用或已覆盖的未定义行为。

动态检测只能覆盖实际执行到的路径。以后实现图片、文件、语音消息或网络连接后，应为对应功能补充可自动退出的测试路径并再次运行 Sanitizer。
