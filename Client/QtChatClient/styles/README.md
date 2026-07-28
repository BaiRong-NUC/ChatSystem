# QtChatClient 样式目录

应用启动时会递归读取 `styles/` 下的所有 `.qss` 文件，并按资源路径排序后合并。

- `base/`：全局基础样式和通用控件规则。
- `main_window/`：主窗口导航、会话列表、聊天区和编辑区样式。
- `windows/`：独立窗口及弹出卡片样式，一个窗口对应一个 QSS 文件。

新增窗口样式时：

1. 为窗口和内部控件设置唯一的 `objectName`。
2. 在 `windows/` 下新增同名 QSS。
3. 把 QSS 加入 `resources/resource.qrc` 的 `styles/windows/` 资源路径。

窗口专用选择器应以窗口对象名开头或使用唯一控件对象名，避免影响其他窗口。
