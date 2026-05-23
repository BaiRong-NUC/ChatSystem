#include <mainwidget.h>

MainWidget *MainWidget::s_instance = nullptr;  // 初始化单例实例指针

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("Qt Chat Client");
    this->setWindowIcon(QIcon(":/images/logo.png"));

    // 初始化窗口资源
    // - 子窗口
    this->m_leftWidget = new QWidget(this);
    this->m_midWidget = new QWidget(this);
    this->m_rightWidget = new QWidget(this);
    this->m_leftWidget->setObjectName("leftWidget");
    this->m_midWidget->setObjectName("midWidget");
    this->m_rightWidget->setObjectName("rightWidget");

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
    this->_InitUI();

    // 初始化信号槽,在UI资源初始化完毕后
    this->_InitSignalSlots();
}

MainWidget::~MainWidget() = default;

MainWidget *MainWidget::GetInstance()
{
    // 不需要多线程,在有多线程前已经创建好了实例,所以不需要加锁
    if (MainWidget::s_instance == nullptr)
    {
        // 默认桌面为父窗口
        MainWidget::s_instance = new MainWidget(nullptr);
    }
    return MainWidget::s_instance;
}

void MainWidget::_InitMainWidget()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    this->setLayout(mainLayout);

    if (this->m_leftWidget == nullptr || this->m_midWidget == nullptr || this->m_rightWidget == nullptr)
    {
        LogInfo(LogLevel::ERROR, "主窗口初始化失败:子窗口指针为nullptr");
        exit(-1);
    }

    this->m_leftWidget->setFixedWidth(50);      // 设置左侧导航栏固定宽度
    this->m_midWidget->setFixedWidth(200);      // 设置中间会话区固定宽度
    this->m_rightWidget->setMinimumWidth(350);  // 设置右侧信息区最小高度,宽度,根据窗口大小自动调整
    this->m_rightWidget->setMinimumHeight(350);
    mainLayout->setContentsMargins(0, 0, 0, 0);  // 设置布局边距为0,让子窗口占满整个主窗口
    mainLayout->setSpacing(0);                   // 设置布局间距为0,让子窗口紧密排列在一起

    // 将背景色限定到三栏容器本身,避免样式级联影响左侧按钮图标显示
    this->setStyleSheet(
        "QWidget#leftWidget { background-color: #000000; }"
        "QWidget#midWidget { background-color: #949ea9; }"
        "QWidget#rightWidget { background-color: #ecf0f1; }");
    mainLayout->addWidget(this->m_leftWidget);
    mainLayout->addWidget(this->m_midWidget);
    mainLayout->addWidget(this->m_rightWidget, 1);

    this->resize(800, 700);
}

void MainWidget::_InitLeftWidget(const ActiveTab &activeTab)
{
    QVBoxLayout *leftLayout = new QVBoxLayout(this->m_leftWidget);
    leftLayout->setContentsMargins(0, 10, 0, 10);
    leftLayout->setSpacing(14);  // 设置按钮之间的间距为14像素
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
            "QPushButton:hover { background: rgba(255, 255, 255, 0.08); border-radius: 10px; }");
    };

    // 头像
    initNavButton(this->m_avatarButton, QSize(44, 44), QSize(36, 36));
    // 会话
    initNavButton(this->m_sessionTabButton, QSize(42, 42), QSize(30, 30));
    // 好友
    initNavButton(this->m_friendTabButton, QSize(42, 42), QSize(30, 30));
    // 好友申请
    initNavButton(this->m_friendRequestTabButton, QSize(42, 42), QSize(30, 30));

    leftLayout->addWidget(this->m_avatarButton);
    leftLayout->addSpacing(5);
    leftLayout->addWidget(this->m_sessionTabButton);
    leftLayout->addWidget(this->m_friendTabButton);
    leftLayout->addWidget(this->m_friendRequestTabButton);
    leftLayout->addStretch();

    // 如果有默认激活标签,则更新按钮状态和当前激活标签
    if (activeTab != None) { this->_UpdateTabButtonStates(activeTab); }
}

void MainWidget::_InitMidWidget() {}

void MainWidget::_InitRightWidget() {}

void MainWidget::_InitUI()
{
    // 初始化主窗口
    this->_InitMainWidget();
    // 初始化左侧导航栏,默认打开会话标签
    this->_InitLeftWidget(ActiveTab::SessionTab);
    // 初始化中间会话区
    this->_InitMidWidget();
    // 初始化右侧信息区
    this->_InitRightWidget();
}

void MainWidget::_UpdateTabButtonStates(const ActiveTab &activeTab)
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
}

void MainWidget::_OnSessionTabClicked()
{
    if (this->m_activeTab == SessionTab) { return; }

    // 更新按钮图标状态和当前激活标签
    this->_UpdateTabButtonStates(SessionTab);
}

void MainWidget::_OnFriendTabClicked()
{
    if (this->m_activeTab == FriendTab) { return; }

    // 更新按钮图标状态和当前激活标签
    this->_UpdateTabButtonStates(FriendTab);
}

void MainWidget::_OnFriendRequestTabClicked()
{
    if (this->m_activeTab == FriendRequestTab) { return; }

    // 更新按钮图标状态和当前激活标签
    this->_UpdateTabButtonStates(FriendRequestTab);
}
void MainWidget::_InitSignalSlots()
{
    // 左侧导航栏按钮点击事件连接到对应的槽函数
    connect(this->m_sessionTabButton, &QPushButton::clicked, this, &MainWidget::_OnSessionTabClicked);
    connect(this->m_friendTabButton, &QPushButton::clicked, this, &MainWidget::_OnFriendTabClicked);
    connect(this->m_friendRequestTabButton, &QPushButton::clicked, this, &MainWidget::_OnFriendRequestTabClicked);
}