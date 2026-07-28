#include <widget/rightwidget/right_widget_title.h>

using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr auto kTitleBarStyle =
        "QWidget#titleBar { background-color: #191919; border-bottom: 1px solid #303030;}";

    constexpr auto kTitleBarLabelStyle =
        "QLabel { font-size: 21px; font-weight: 500; color: #f0f0f0; background: transparent; }";

    constexpr auto kTitleBarButtonStyle =
        "QPushButton { color: #d8d8d8; background-color: transparent; border: none; border-radius: 5px;"
        " font-size: 20px; padding-bottom: 8px; }"
        "QPushButton:hover { background-color: #2b2b2b; }"
        "QPushButton:pressed { background-color: #343434; }";
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
    this->setFixedHeight(74);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setObjectName("titleBar");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet(kTitleBarStyle);

    // 标签有一个label和一个按钮
    QHBoxLayout *titleBarLayout = new QHBoxLayout(this);
    titleBarLayout->setContentsMargins(24, 0, 18, 0);
    titleBarLayout->setSpacing(0);
    // - 标签
    this->m_titleLabel->setStyleSheet(kTitleBarLabelStyle);

#if DEBUG_CODE
    this->m_titleLabel->setText("好友1");
#endif

    titleBarLayout->addWidget(this->m_titleLabel);
    // - 按钮
    this->m_titleButton->setFixedSize(42, 36);
    this->m_titleButton->setText("•••");
    this->m_titleButton->setStyleSheet(kTitleBarButtonStyle);
    titleBarLayout->addWidget(this->m_titleButton);
}
