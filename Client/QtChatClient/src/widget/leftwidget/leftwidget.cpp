#include <widget/leftwidget/leftwidget.h>

using namespace ChatWidget;
using namespace Log;

LeftWidget *LeftWidget::s_instance = nullptr;  // 初始化单例实例指针

LeftWidget *LeftWidget::GetInstance(QWidget *parent)
{
    if (s_instance == nullptr) { s_instance = new LeftWidget(parent); }
    return s_instance;
}

LeftWidget::LeftWidget(QWidget *parent) : QWidget(parent)
{
    // 初始化窗口资源

    // - 按钮
    this->m_avatarButton = new QPushButton(this);
    this->m_sessionTabButton = new QPushButton(this);
    this->m_friendTabButton = new QPushButton(this);
    this->m_friendRequestTabButton = new QPushButton(this);

    // - Icon资源映射
    this->m_tabIconInfoMap = {
        {this->m_sessionTabButton, {":/images/session_active.png", ":/images/session_inactive.png"}},
        {this->m_friendTabButton, {":/images/friend_active.png", ":/images/friend_inactive.png"}},
        {this->m_friendRequestTabButton, {":/images/apply_active.png", ":/images/apply_inactive.png"}},
    };

    // 初始化UI界面
    this->_InitLeftWidget(ActiveTab::SessionTab);  // 默认激活会话标签

    // 初始化信号槽,在UI资源初始化完毕后
    this->_InitSignalSlots();
}

LeftWidget::~LeftWidget()
{
    if (this->m_avatarButton != nullptr) { delete this->m_avatarButton; }
    if (this->m_sessionTabButton != nullptr) { delete this->m_sessionTabButton; }
    if (this->m_friendTabButton != nullptr) { delete this->m_friendTabButton; }
    if (this->m_friendRequestTabButton != nullptr) { delete this->m_friendRequestTabButton; }
    this->m_avatarButton = nullptr;
    this->m_sessionTabButton = nullptr;
    this->m_friendTabButton = nullptr;
    this->m_friendRequestTabButton = nullptr;
}

void LeftWidget::_InitLeftWidget(const ActiveTab &activeTab)
{
    this->setFixedWidth(72);  // 接近桌面聊天软件的窄导航栏比例
    // 设置背景
    this->setObjectName("leftWidget");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet("QWidget#leftWidget { background-color: #171717; border-right: 1px solid #202020; }");

    QVBoxLayout *leftLayout = new QVBoxLayout(this);
    leftLayout->setContentsMargins(0, 18, 0, 18);
    leftLayout->setSpacing(14);
    leftLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    // 创建按钮
    if (this->m_avatarButton == nullptr || this->m_sessionTabButton == nullptr || this->m_friendTabButton == nullptr ||
        this->m_friendRequestTabButton == nullptr)
    {
        LogInfo(LogLevel::ERROR, "主窗口初始化失败:左侧导航栏按钮指针为nullptr");
        exit(-1);
    }
    auto initNavButton = [this](QPushButton *button, const QSize &buttonSize, const QSize &iconSize)
    {
        button->setFixedSize(buttonSize);
        button->setIconSize(iconSize);
        const char *iconPath = this->m_tabIconInfoMap.contains(button) ? this->m_tabIconInfoMap[button].inactiveIcon
                                                                       : ":/images/defaultAvatar.png";
        button->setIcon(QIcon(iconPath));
        button->setFlat(true);
        button->setCursor(Qt::PointingHandCursor);  // 鼠标悬停时显示手型光标
        button->setStyleSheet(
            "QPushButton { background: transparent; border: none; }"
            "QPushButton:hover { background: #282828; border-radius: 6px; }"
            "QPushButton:pressed { background: #303030; border-radius: 6px; }");
    };

    // 头像
    initNavButton(this->m_avatarButton, QSize(48, 48), QSize(42, 42));
    // 会话
    initNavButton(this->m_sessionTabButton, QSize(46, 46), QSize(30, 30));
    // 好友
    initNavButton(this->m_friendTabButton, QSize(46, 46), QSize(30, 30));
    // 好友申请
    initNavButton(this->m_friendRequestTabButton, QSize(46, 46), QSize(30, 30));

    leftLayout->addWidget(this->m_avatarButton);
    leftLayout->addSpacing(5);
    leftLayout->addWidget(this->m_sessionTabButton);
    leftLayout->addWidget(this->m_friendTabButton);
    leftLayout->addWidget(this->m_friendRequestTabButton);
    leftLayout->addStretch();

    // 如果有默认激活标签,则更新按钮状态和当前激活标签和中间会话区显示内容
    if (activeTab != None) { this->_SwitchTabButton(activeTab); }
}

void LeftWidget::_SwitchTabButton(const ActiveTab &activeTab)
{
    if (this->m_activeTab == activeTab) { return; }
    this->m_activeTab = activeTab;

    auto setButtonIcon = [this](QPushButton *button, bool active)
    {
        auto it = this->m_tabIconInfoMap.find(button);
        if (it != this->m_tabIconInfoMap.end()) { button->setIcon(QIcon(active ? it->activeIcon : it->inactiveIcon)); }
    };

    setButtonIcon(this->m_sessionTabButton, activeTab == SessionTab);
    setButtonIcon(this->m_friendTabButton, activeTab == FriendTab);
    setButtonIcon(this->m_friendRequestTabButton, activeTab == FriendRequestTab);

    // TODO: 切换到会话标签,更新中间会话区显示内容
}

void LeftWidget::_OnSessionTabClicked()
{
    if (this->m_activeTab == SessionTab) { return; }

    // 更新按钮图标状态和当前激活标签
    this->_SwitchTabButton(SessionTab);
}

void LeftWidget::_OnFriendTabClicked()
{
    if (this->m_activeTab == FriendTab) { return; }

    // 更新按钮图标状态和当前激活标签
    this->_SwitchTabButton(FriendTab);
}

void LeftWidget::_OnFriendRequestTabClicked()
{
    if (this->m_activeTab == FriendRequestTab) { return; }

    // 更新按钮图标状态和当前激活标签
    this->_SwitchTabButton(FriendRequestTab);
}
void LeftWidget::_InitSignalSlots()
{
    // 左侧导航栏按钮点击事件连接到对应的槽函数
    connect(this->m_sessionTabButton, &QPushButton::clicked, this, &LeftWidget::_OnSessionTabClicked);
    connect(this->m_friendTabButton, &QPushButton::clicked, this, &LeftWidget::_OnFriendTabClicked);
    connect(this->m_friendRequestTabButton, &QPushButton::clicked, this, &LeftWidget::_OnFriendRequestTabClicked);
}
