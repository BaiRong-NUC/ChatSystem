#include <widget/mainwidget.h>

MainWidget *MainWidget::s_instance = nullptr;  // 初始化单例实例指针

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    // 设置主窗口标题和图标
    this->setWindowTitle("Qt Chat Client");
    this->setWindowIcon(QIcon(":/images/logo.png"));

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
    mainLayout->setContentsMargins(0, 0, 0, 0);  // 设置布局边距为0,让子窗口占满整个主窗口
    mainLayout->setSpacing(0);                   // 设置布局间距为0,让子窗口紧密排列在一起

    mainLayout->addWidget(this->m_leftWidget);
    mainLayout->addWidget(this->m_midWidget);
    mainLayout->addWidget(this->m_rightWidget);
}
