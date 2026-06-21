#include <widget/rightwidget/rightwidget.h>
using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr auto kRightWidgetStyle = "QWidget#rightWidget { background-color: #ecf0f1; }";

    constexpr auto kTitleBarStyle = "QWidget#titleBar { background-color: #bdc3c7; }";

    constexpr auto kTitleBarLabelStyle = "QLabel { font-size: 18px; font-weight: bold; color: #2c3e50; }";

    constexpr auto kTitleBarButtonIconPath = ":/images/more.png";
    constexpr auto kTitleBarButtonStyle =
        "QPushButton { background-color: transparent; border: none; border-radius: 4px; }"
        "QPushButton:hover { background-color: #ecf0f1; border: none; }"
        "QPushButton:pressed { background-color: #bdc3c7; border: none; }";
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
    this->m_titleBar = new QWidget(this);
    this->m_titleLabel = new QLabel(this->m_titleBar);
    this->m_titleButton = new QPushButton(this->m_titleBar);

    // 初始化UI界面
    this->_InitRightWidget();
}

RightWidget::~RightWidget() = default;

void RightWidget::_InitRightWidget()
{
    if (this->m_titleBar == nullptr || this->m_titleLabel == nullptr || this->m_titleButton == nullptr)
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
    this->setLayout(rightLayout);

    // 上方标题栏
    this->m_titleBar->setFixedHeight(50);
    this->m_titleBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->m_titleBar->setObjectName("titleBar");
    this->m_titleBar->setAttribute(Qt::WA_StyledBackground, true);
    this->m_titleBar->setStyleSheet(kTitleBarStyle);
    rightLayout->addWidget(this->m_titleBar);
    // 标签有一个label和一个按钮
    QHBoxLayout *titleBarLayout = new QHBoxLayout(this->m_titleBar);
    titleBarLayout->setContentsMargins(0, 0, 0, 0);
    titleBarLayout->setSpacing(0);
    // - 标签
    this->m_titleLabel->setStyleSheet(kTitleBarLabelStyle);
#if DEBUG_CODE
    this->m_titleLabel->setText("聊天信息");
#endif
    titleBarLayout->addWidget(this->m_titleLabel);
    // - 按钮
    this->m_titleButton->setFixedSize(30, 30);
    this->m_titleButton->setIconSize(QSize(30, 30));
    this->m_titleButton->setIcon(QIcon(kTitleBarButtonIconPath));
    this->m_titleButton->setStyleSheet(kTitleBarButtonStyle);
    titleBarLayout->addWidget(this->m_titleButton);
}