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

    // 初始化UI界面
    this->_InitUI();
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

void MainWidget::_InitLeftWidget()
{
    QVBoxLayout *leftLayout = new QVBoxLayout(this->m_leftWidget);
    leftLayout->setSpacing(0);
    // 创建按钮
    if (this->m_avatarButton == nullptr || this->m_sessionTabButton == nullptr || this->m_friendTabButton == nullptr ||
        this->m_friendRequestTabButton == nullptr)
    {
        LogInfo(LogLevel::ERROR, "主窗口初始化失败:左侧导航栏按钮指针为nullptr");
        exit(-1);
    }
    auto initNavButton = [](QPushButton *button, const QString &iconPath)
    {
        button->setFixedSize(45, 45);
        button->setIconSize(QSize(45, 45));
        button->setIcon(QIcon(iconPath));
        button->setFlat(true);
    };

    // 头像
    initNavButton(this->m_avatarButton, ":/images/defaultAvatar.png");
    // 会话
    initNavButton(this->m_sessionTabButton, ":/images/session_active.png");
    // 好友
    initNavButton(this->m_friendTabButton, ":/images/friend_inactive.png");
    // 好友申请
    initNavButton(this->m_friendRequestTabButton, ":/images/apply_inactive.png");

    leftLayout->addWidget(this->m_avatarButton, 1, Qt::AlignTop | Qt::AlignHCenter);
    leftLayout->addWidget(this->m_sessionTabButton, 1, Qt::AlignTop | Qt::AlignHCenter);
    leftLayout->addWidget(this->m_friendTabButton, 1, Qt::AlignTop | Qt::AlignHCenter);
    leftLayout->addWidget(this->m_friendRequestTabButton, 1, Qt::AlignTop | Qt::AlignHCenter);
}

void MainWidget::_InitMidWidget() {}

void MainWidget::_InitRightWidget() {}

void MainWidget::_InitUI()
{
    // 初始化主窗口
    this->_InitMainWidget();
    // 初始化左侧导航栏
    this->_InitLeftWidget();
    // 初始化中间会话区
    this->_InitMidWidget();
    // 初始化右侧信息区
    this->_InitRightWidget();
}
