#include <widget/rightwidget/rightwidget.h>
using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr auto kRightWidgetStyle = "QWidget#rightWidget { background-color: #ecf0f1; }";
}  // namespace

RightWidget *RightWidget::s_instance = nullptr;  // 初始化单例实例指针

RightWidget *RightWidget::GetInstance(QWidget *parent)
{
    if (s_instance == nullptr) { s_instance = new RightWidget(parent); }
    return s_instance;
}

RightWidget::RightWidget(QWidget *parent) : QWidget(parent)
{
    // 初始化资源
    this->m_titleBar = RightWidgetTitle::GetInstance(this);    // 获取标题栏单例实例
    this->m_messageWidget = MessageWidget::GetInstance(this);  // 获取聊天窗口单例实例

    // 初始化UI界面
    this->_InitRightWidget();
}

RightWidget::~RightWidget() = default;

void RightWidget::_InitRightWidget()
{
    if (this->m_titleBar == nullptr || this->m_messageWidget == nullptr)
    {
        LogInfo(LogLevel::ERROR, "右侧信息区资源初始化失败");
        exit(-1);
    }

    this->setObjectName("rightWidget");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setMinimumWidth(350);  // 设置右侧信息区最小高度,宽度,根据窗口大小自动调整
    this->setMinimumHeight(350);
    // 背景
    this->setStyleSheet(kRightWidgetStyle);
    // 布局
    QVBoxLayout *rightLayout = new QVBoxLayout(this);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);
    rightLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    this->setLayout(rightLayout);

    // 初始化标题栏
    rightLayout->addWidget(this->m_titleBar);

    // 初始化聊天窗口
    rightLayout->addWidget(this->m_messageWidget);
}
