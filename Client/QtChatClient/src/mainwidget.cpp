#include <mainwidget.h>

MainWidget *MainWidget::s_instance = nullptr;  // 初始化单例实例指针

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("Qt Chat Client");
    this->setWindowIcon(QIcon(":/images/logo.png"));
    this->m_leftWidget = new QWidget(this);
    this->m_midWidget = new QWidget(this);
    this->m_rightWidget = new QWidget(this);

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

    // 设置窗口颜色
    this->m_leftWidget->setStyleSheet("background-color: #000000;");
    this->m_midWidget->setStyleSheet("background-color: #949ea9;");
    this->m_rightWidget->setStyleSheet("background-color: #ecf0f1;");
    mainLayout->addWidget(this->m_leftWidget);
    mainLayout->addWidget(this->m_midWidget);
    mainLayout->addWidget(this->m_rightWidget, 1);

    this->resize(800, 700);
}

void MainWidget::_InitLeftWidget() {}

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
