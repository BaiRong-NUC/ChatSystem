#include <widget/main_widget/midwidget/midwidget.h>
using namespace ChatWidget;
using namespace Log;

MidWidget::MidWidget(QWidget *parent) : QWidget(parent)
{
    // 初始化窗口资源
    this->m_titleBar = new MidWidgetTitle(this);            // 创建标题栏
    this->m_sessionArea = new MidSessionAreaWidget(this);  // 创建会话

    // 初始化UI界面
    this->_InitMidWidget();

    // 初始化信号槽,在UI资源初始化完毕后
    this->_InitSignalSlots();
}

MidWidget::~MidWidget() = default;

void MidWidget::_InitMidWidget()
{
    if (this->m_titleBar == nullptr || this->m_sessionArea == nullptr)
    {
        LogInfo(LogLevel::ERROR, "MidWidget中间会话区资源初始化失败");
        exit(-1);
    }

    // 窗口名称,大小和背景
    this->setObjectName("midWidget");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setFixedWidth(270);

    // 标题栏和会话列表横向铺满中栏，避免滚动区按sizeHint居中产生大块左右留白。
    QVBoxLayout *midLayout = new QVBoxLayout(this);
    midLayout->setContentsMargins(0, 0, 0, 0);
    midLayout->setSpacing(0);
    midLayout->setAlignment(Qt::AlignTop);
    this->setLayout(midLayout);

    // 标题栏
    midLayout->addWidget(this->m_titleBar);
    // 好友信息框
    midLayout->addWidget(this->m_sessionArea, 1);

#if DEBUG_CODE
    LogInfo(LogLevel::INFO, "MidWidget::_InitMidWidget() 添加测试项");
    this->m_sessionArea->SelectSessionItem(0);
#endif
}

void MidWidget::_InitSignalSlots()
{
    connect(this->m_titleBar, &MidWidgetTitle::addFriendRequested, this, &MidWidget::addFriendRequested);
}
