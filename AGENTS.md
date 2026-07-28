# C++ 内存管理约定

本约定适用于本仓库内所有 C++ 后端代码和 Qt 代码。

- 能明确所有权时优先使用智能指针，不新增负责所有权的裸指针。
- 非 `QObject` 对象由单一所有者管理时，优先使用 `std::unique_ptr` 和 `std::make_unique`。
- 只有确实存在共享所有权时才使用 `std::shared_ptr` 和 `std::make_shared`，不要为了传参方便使用共享所有权。
- 带有父对象的 `QObject` 由 Qt 对象树管理；需要保存为成员观察指针时使用 `QPointer<T>`，不要再用标准智能指针重复持有。
- 没有父对象且需要独占所有权的 `QObject`，使用 `std::unique_ptr<T>`；将对象交给 Qt 父对象、布局或其他 Qt 所有者后，应明确转移所有权。
- `layout()`、`widget()`、`parent()` 等 API 返回的非拥有临时指针允许使用裸指针，不得释放。
- 优先使用栈对象和值语义；只有生命周期或多态需求确实要求动态分配时才使用堆对象。
- 修改既有代码时，在不扩大任务范围且不会改变正确所有权语义的前提下，将可替换的拥有型裸指针改为合适的智能指针。
- 禁止让 Qt 父对象和 `std::unique_ptr`/`std::shared_ptr` 同时拥有同一 `QObject`，避免重复释放。

Qt 客户端的详细说明和检测命令见 `Client/QtChatClient/md/memory-safety.md`。
