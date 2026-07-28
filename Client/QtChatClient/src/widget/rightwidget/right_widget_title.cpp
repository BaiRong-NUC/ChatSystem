#include <widget/rightwidget/right_widget_title.h>
#include <QMouseEvent>
#include <QWindow>

using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr auto kTitleBarStyleSheet =
        "QWidget#titleBar { background-color: #191919; border-bottom: 1px solid #303030; }"
        "QPushButton#windowButton { background-color: transparent; border: none; }"
        "QPushButton#windowButton:hover { background-color: #2b2b2b; }"
        "QPushButton#windowButton:pressed, QPushButton#windowButton:checked { background-color: #353535; }"
        "QPushButton#closeButton { background-color: transparent; border: none; }"
        "QPushButton#closeButton:hover { background-color: #c42b1c; }"
        "QPushButton#closeButton:pressed { background-color: #a62316; }";

    constexpr auto kTitleBarLabelStyleSheet =
        "QLabel { font-size: 21px; font-weight: 500; color: #f0f0f0; background: transparent; }";

    constexpr auto kMoreButtonStyleSheet =
        "QPushButton { background-color: transparent; border: none; border-radius: 5px; padding: 0px; }"
        "QPushButton:hover { background-color: #2b2b2b; }"
        "QPushButton:pressed { background-color: #343434; }";

    constexpr auto kPinIconPath = ":/icons/tabler/pin.svg";
    constexpr auto kMinimizeIconPath = ":/icons/tabler/minus.svg";
    constexpr auto kMaximizeIconPath = ":/icons/tabler/square.svg";
    constexpr auto kRestoreIconPath = ":/icons/tabler/copy.svg";
    constexpr auto kCloseIconPath = ":/icons/tabler/x.svg";
    constexpr auto kMoreIconPath = ":/icons/tabler/dots.svg";
}  // namespace

RightWidgetTitle::RightWidgetTitle(QWidget *parent) : QWidget(parent)
{
    // 初始化资源
    this->m_titleLabel = new QLabel(this);
    this->m_titleButton = new QPushButton(this);
    this->m_pinButton = new QPushButton(this);
    this->m_minimizeButton = new QPushButton(this);
    this->m_maximizeButton = new QPushButton(this);
    this->m_closeButton = new QPushButton(this);

    // 初始化UI界面
    this->_InitRightWidgetTitle();
    this->_InitSignalSlots();
}

RightWidgetTitle::~RightWidgetTitle() = default;

void RightWidgetTitle::_InitRightWidgetTitle()
{
    if (this->m_titleLabel == nullptr || this->m_titleButton == nullptr || this->m_pinButton == nullptr ||
        this->m_minimizeButton == nullptr || this->m_maximizeButton == nullptr || this->m_closeButton == nullptr)
    {
        LogInfo(LogLevel::ERROR, "右侧信息区标题栏资源初始化失败");
        exit(-1);
    }
    // 上方标题栏
    this->setFixedHeight(74);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setObjectName("titleBar");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet(kTitleBarStyleSheet);

    QVBoxLayout *titleBarLayout = new QVBoxLayout(this);
    titleBarLayout->setContentsMargins(24, 0, 0, 0);
    titleBarLayout->setSpacing(0);

    // 第一行：窗口置顶、最小化、最大化、关闭。
    QHBoxLayout *windowControlLayout = new QHBoxLayout();
    windowControlLayout->setContentsMargins(0, 0, 0, 0);
    windowControlLayout->setSpacing(0);
    windowControlLayout->addStretch();

    const QList<QPushButton *> windowButtons = {
        this->m_pinButton,
        this->m_minimizeButton,
        this->m_maximizeButton,
        this->m_closeButton,
    };
    for (QPushButton *button : windowButtons)
    {
        button->setObjectName("windowButton");
        button->setFixedSize(38, 28);
        button->setIconSize(QSize(15, 15));
        button->setCursor(Qt::PointingHandCursor);
        windowControlLayout->addWidget(button);
    }
    this->m_closeButton->setObjectName("closeButton");
    this->m_pinButton->setCheckable(true);
    this->m_pinButton->setIcon(QIcon(kPinIconPath));
    this->m_minimizeButton->setIcon(QIcon(kMinimizeIconPath));
    this->_UpdateMaximizeButtonIcon();
    this->m_closeButton->setIcon(QIcon(kCloseIconPath));
    titleBarLayout->addLayout(windowControlLayout);

    // 第二行：会话标题和更多按钮。
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 18, 0);
    contentLayout->setSpacing(0);
    this->m_titleLabel->setStyleSheet(kTitleBarLabelStyleSheet);

#if DEBUG_CODE
    this->m_titleLabel->setText("好友1");
#endif

    contentLayout->addWidget(this->m_titleLabel);
    contentLayout->addStretch();
    this->m_titleButton->setFixedSize(42, 36);
    this->m_titleButton->setIcon(QIcon(kMoreIconPath));
    this->m_titleButton->setIconSize(QSize(22, 22));
    this->m_titleButton->setStyleSheet(kMoreButtonStyleSheet);
    contentLayout->addWidget(this->m_titleButton);
    titleBarLayout->addLayout(contentLayout);
}

void RightWidgetTitle::_InitSignalSlots()
{
    connect(this->m_pinButton, &QPushButton::toggled, this, &RightWidgetTitle::_ToggleAlwaysOnTop);
    connect(this->m_minimizeButton, &QPushButton::clicked, this, [this]() { this->window()->showMinimized(); });
    connect(this->m_maximizeButton, &QPushButton::clicked, this, &RightWidgetTitle::_ToggleMaximized);
    connect(this->m_closeButton, &QPushButton::clicked, this, [this]() { this->window()->close(); });
}

void RightWidgetTitle::_ToggleMaximized()
{
    if (this->window()->isMaximized())
    {
        this->window()->showNormal();
    }
    else
    {
        this->window()->showMaximized();
    }
    this->_UpdateMaximizeButtonIcon();
}

void RightWidgetTitle::_ToggleAlwaysOnTop(bool checked)
{
    QWidget *topLevelWindow = this->window();
    const bool wasMaximized = topLevelWindow->isMaximized();
    topLevelWindow->setWindowFlag(Qt::WindowStaysOnTopHint, checked);
    if (wasMaximized)
    {
        topLevelWindow->showMaximized();
    }
    else
    {
        topLevelWindow->show();
    }
}

void RightWidgetTitle::_UpdateMaximizeButtonIcon()
{
    this->m_maximizeButton->setIcon(
        QIcon(this->window()->isMaximized() ? kRestoreIconPath : kMaximizeIconPath));
}

void RightWidgetTitle::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && this->window()->windowHandle() != nullptr)
    {
        this->window()->windowHandle()->startSystemMove();
    }
    QWidget::mousePressEvent(event);
}

void RightWidgetTitle::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) { this->_ToggleMaximized(); }
    QWidget::mouseDoubleClickEvent(event);
}
