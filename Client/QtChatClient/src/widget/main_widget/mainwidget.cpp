#include <widget/main_widget/mainwidget.h>

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

void MainWidget::_InitMainWidget()
{
    this->setObjectName("mainWidget");
    this->setAttribute(Qt::WA_StyledBackground, true);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    this->setLayout(mainLayout);

    this->setMinimumSize(960, 640);
    this->resize(1200, 760);

    // 创建子窗口
    this->m_leftWidget = new LeftWidget(this);
    this->m_midWidget = new MidWidget(this);
    this->m_rightWidget = new RightWidget(this);

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
