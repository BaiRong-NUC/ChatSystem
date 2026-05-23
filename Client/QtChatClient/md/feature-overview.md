# QtChatClient 功能说明

本文档描述 QtChatClient 当前已经实现的界面结构、功能模块与行为逻辑。

## 1. 当前功能范围

当前项目已完成的能力主要集中在基础界面骨架和导航状态管理：

1. 主窗口单例创建与展示。
2. 主窗口拆分为 MainWidget、LeftWidget、MidWidget、RightWidget 四个窗口类。
3. 左中右三栏窗口骨架搭建。
4. 左侧导航栏按钮初始化。
5. 会话、好友、好友申请三个标签的图标切换逻辑。
6. 基础日志输出与公共工具支持。

当前尚未完成的内容包括：

1. 中间列表区具体业务 UI。
2. 右侧聊天详情或信息面板。
3. 会话数据加载与消息展示。
4. 网络通信、登录、消息收发等完整聊天流程。

## 2. 主窗口模块

当前客户端主窗口由 MainWidget 负责装配，声明位于 include/widget/mainwidget.h，实现位于 src/widget/mainwidget.cpp。

### 2.1 设计概览

MainWidget 继承 QWidget，当前采用单例模式对外提供唯一窗口实例，同时只负责三栏子窗口的创建、持有和布局装配：

| 成员/方法        | 类型                | 作用                         |
| ---------------- | ------------------- | ---------------------------- |
| s_instance       | MainWidget\*        | 保存单例实例                 |
| GetInstance      | static MainWidget\* | 获取主窗口唯一实例           |
| \_InitMainWidget | void                | 初始化主布局并装配三个子窗口 |
| m_leftWidget     | LeftWidget\*        | 持有左侧导航栏窗口           |
| m_midWidget      | MidWidget\*         | 持有中间列表区窗口           |
| m_rightWidget    | RightWidget\*       | 持有右侧内容区窗口           |

当前程序入口 main.cpp 中会创建 QApplication，并通过 MainWidget::GetInstance 获取主窗口后调用 show 展示界面。

### 2.2 窗口布局结构

主窗口当前使用三栏布局：

| 区域成员      | 类型          | 当前职责                                 |
| ------------- | ------------- | ---------------------------------------- |
| m_leftWidget  | LeftWidget\*  | 左侧导航栏，承载头像与标签按钮           |
| m_midWidget   | MidWidget\*   | 中间列表区，后续用于会话、好友、申请列表 |
| m_rightWidget | RightWidget\* | 右侧主内容区，后续用于聊天详情或信息面板 |

布局初始化由 \_InitMainWidget 完成，当前行为如下：

1. 使用 QHBoxLayout 将 LeftWidget、MidWidget、RightWidget 横向排列。
2. MainWidget 只负责创建并加入三个子窗口，不再承载各区域具体 UI 细节。
3. 主布局边距与间距均为 0。
4. 左侧、中间、右侧窗口各自维护自己的尺寸约束和内部界面。

当前主窗口默认尺寸为 800 x 700，窗口标题为 Qt Chat Client，窗口图标来自资源文件 :/images/logo.png。

## 3. 左侧导航栏功能

左侧导航栏当前包含 1 个头像按钮和 3 个功能标签按钮：

| 成员                     | 类型          | 作用                           |
| ------------------------ | ------------- | ------------------------------ |
| m_avatarButton           | QPushButton\* | 用户头像入口，当前显示默认头像 |
| m_sessionTabButton       | QPushButton\* | 会话标签按钮                   |
| m_friendTabButton        | QPushButton\* | 好友标签按钮                   |
| m_friendRequestTabButton | QPushButton\* | 好友申请标签按钮               |

当前按钮图标资源由 TabIconInfo 和 m_tabIconInfoMap 维护：

| 按钮     | 激活图标                    | 未激活图标                    |
| -------- | --------------------------- | ----------------------------- |
| 会话     | :/images/session_active.png | :/images/session_inactive.png |
| 好友     | :/images/friend_active.png  | :/images/friend_inactive.png  |
| 好友申请 | :/images/apply_active.png   | :/images/apply_inactive.png   |

头像按钮当前不参与标签切换逻辑，默认使用 :/images/defaultAvatar.png。

## 4. 标签状态切换逻辑

LeftWidget 内部定义了 ActiveTab 枚举：

| 枚举值           | 含义         |
| ---------------- | ------------ |
| None             | 无激活标签   |
| SessionTab       | 会话标签     |
| FriendTab        | 好友标签     |
| FriendRequestTab | 好友申请标签 |

当前行为如下：

1. \_InitLeftWidget 默认以 SessionTab 作为启动后的激活标签。
2. 点击不同标签按钮时，进入对应槽函数。
3. 若重复点击当前激活标签，则直接返回，不重复刷新。
4. \_SwitchTabButton 会同步更新 m_activeTab 和三枚按钮图标。
5. 中间区域内容切换尚未实现，代码中保留了 TODO。

## 5. 初始化顺序

当前窗口初始化顺序如下：

1. 设置窗口标题与窗口图标。
2. MainWidget 调用 \_InitMainWidget 创建主布局。
3. MainWidget 获取 LeftWidget、MidWidget、RightWidget 三个子窗口实例并加入布局。
4. LeftWidget 在自身构造阶段创建头像按钮和三个标签按钮。
5. LeftWidget 初始化按钮图标资源映射、默认激活标签和信号槽。
6. MidWidget 和 RightWidget 当前完成基础 objectName、尺寸约束和占位容器初始化。

当前初始化方式说明项目已经从“主窗口集中堆叠全部 UI 代码”调整为“主窗口装配 + 子窗口各自负责自身界面”的结构。

## 6. 信号槽绑定

当前已完成的信号槽连接全部位于 \_InitSignalSlots 中：

| 信号发送者               | 信号    | 槽函数                      |
| ------------------------ | ------- | --------------------------- |
| m_sessionTabButton       | clicked | \_OnSessionTabClicked       |
| m_friendTabButton        | clicked | \_OnFriendTabClicked        |
| m_friendRequestTabButton | clicked | \_OnFriendRequestTabClicked |

三个槽函数目前只负责判断是否重复点击并调用 \_SwitchTabButton 完成状态切换。
