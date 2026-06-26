#include <widget/rightwidget/right_widget_title.h>

using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr auto kTitleBarStyle =
        "QWidget#titleBar { background-color: #bdc3c7; border-bottom: 1px solid #43414147;}";

    constexpr auto kTitleBarLabelStyle = "QLabel { font-size: 18px; font-weight: bold; color: #2c3e50; }";

    constexpr auto kTitleBarButtonIconPath = ":/images/more.png";
    constexpr auto kTitleBarButtonStyle =
        "QPushButton { background-color: transparent; border: none; border-radius: 4px; }"
        "QPushButton:hover { background-color: #ecf0f1; border: none; }"
        "QPushButton:pressed { background-color: #bdc3c7; border: none; }";
}  // namespace

RightWidgetTitle *RightWidgetTitle::s_instance = nullptr;  // 初始化单例实例指针

RightWidgetTitle *RightWidgetTitle::GetInstance(QWidget *parent)
{
    if (s_instance == nullptr) { s_instance = new RightWidgetTitle(parent); }
    return s_instance;
}

RightWidgetTitle::RightWidgetTitle(QWidget *parent) : QWidget(parent)
{
    // 初始化资源
    this->m_titleLabel = new QLabel(this);
    this->m_titleButton = new QPushButton(this);

    // 初始化UI界面
    this->_InitRightWidgetTitle();
}

RightWidgetTitle::~RightWidgetTitle()
{
    if (this->m_titleLabel != nullptr) { delete this->m_titleLabel; }
    if (this->m_titleButton != nullptr) { delete this->m_titleButton; }
    this->m_titleLabel = nullptr;
    this->m_titleButton = nullptr;
}

void RightWidgetTitle::_InitRightWidgetTitle()
{
    if (this->m_titleLabel == nullptr || this->m_titleButton == nullptr)
    {
        LogInfo(LogLevel::ERROR, "右侧信息区标题栏资源初始化失败");
        exit(-1);
    }
    // 上方标题栏
    this->setFixedHeight(58);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setObjectName("titleBar");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet(kTitleBarStyle);

    // 标签有一个label和一个按钮
    QHBoxLayout *titleBarLayout = new QHBoxLayout(this);
    titleBarLayout->setContentsMargins(10, 0, 10, 0);
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