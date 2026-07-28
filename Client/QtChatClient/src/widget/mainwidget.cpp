#include <widget/mainwidget.h>

namespace
{
    constexpr auto kMainWidgetStyleSheet =
        "QWidget#mainWidget { background-color: #191919; border: 1px solid #303030; }";
}  // namespace

MainWidget *MainWidget::s_instance = nullptr;  // 初始化单例实例指针

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    // 设置主窗口标题和图标
    this->setWindowTitle("Qt Chat Client");
    this->setWindowIcon(QIcon(":/images/logo.png"));
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    // 初始化主窗口
    this->_InitMainWidget();
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
    this->setObjectName("mainWidget");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet(kMainWidgetStyleSheet);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    this->setLayout(mainLayout);

    this->setMinimumSize(960, 640);
    this->resize(1200, 760);

    // 创建子窗口
    this->m_leftWidget = LeftWidget::GetInstance(this);
    this->m_midWidget = MidWidget::GetInstance(this);
    this->m_rightWidget = RightWidget::GetInstance(this);

    if (this->m_leftWidget == nullptr || this->m_midWidget == nullptr || this->m_rightWidget == nullptr)
    {
        LogInfo(LogLevel::ERROR, "主窗口初始化失败:子窗口指针为nullptr");
        exit(-1);
    }
    mainLayout->setContentsMargins(1, 1, 1, 1);  // 为无边框窗口保留一像素外边框
    mainLayout->setSpacing(0);                   // 设置布局间距为0,让子窗口紧密排列在一起

    mainLayout->addWidget(this->m_leftWidget);
    mainLayout->addWidget(this->m_midWidget);
    mainLayout->addWidget(this->m_rightWidget);
}
