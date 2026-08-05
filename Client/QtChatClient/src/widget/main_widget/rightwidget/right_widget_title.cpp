#include <widget/main_widget/rightwidget/right_widget_title.h>

#include <QGuiApplication>

using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr auto kPinIconPath = ":/images/icons/tabler/pin.svg";
    constexpr auto kMinimizeIconPath = ":/images/icons/tabler/minus.svg";
    constexpr auto kMaximizeIconPath = ":/images/icons/tabler/square.svg";
    constexpr auto kRestoreIconPath = ":/images/icons/tabler/copy.svg";
    constexpr auto kCloseIconPath = ":/images/icons/tabler/x.svg";
    constexpr auto kMoreIconPath = ":/images/icons/tabler/dots.svg";
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
        button->setProperty("variant", "icon");
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
    this->m_titleLabel->setObjectName("chatTitleLabel");

#if DEBUG_CODE
    this->m_titleLabel->setText("好友1");
#endif

    contentLayout->addWidget(this->m_titleLabel);
    contentLayout->addStretch();
    this->m_titleButton->setFixedSize(42, 36);
    this->m_titleButton->setIcon(QIcon(kMoreIconPath));
    this->m_titleButton->setIconSize(QSize(22, 22));
    this->m_titleButton->setObjectName("moreButton");
    this->m_titleButton->setProperty("variant", "icon");
    contentLayout->addWidget(this->m_titleButton);
    titleBarLayout->addLayout(contentLayout);
}

void RightWidgetTitle::_InitSignalSlots()
{
    connect(this->m_pinButton, &QPushButton::toggled, this, &RightWidgetTitle::_ToggleAlwaysOnTop);
    connect(this->m_minimizeButton, &QPushButton::clicked, this, [this]() { this->window()->showMinimized(); });
    connect(this->m_maximizeButton, &QPushButton::clicked, this, &RightWidgetTitle::_ToggleMaximized);
    connect(this->m_closeButton, &QPushButton::clicked, this, [this]() { this->window()->close(); });

    connect(this->m_titleButton, &QPushButton::clicked, this,
            [this]()
            {
                // 打开会话详情窗口
                if (this->m_titleLabel == nullptr || this->m_titleButton == nullptr) { return; }

                if (this->m_sessionDetailWidget == nullptr)
                {
                    // 标题栏当前只持有会话名称，先使用现有展示数据搭建详情窗口。
                    // 待会话模型接入后，再由业务层传入完整 UserInfo。
                    Model::UserInfo userInfo;
                    userInfo.m_userName = this->m_titleLabel->text().trimmed();
                    if (userInfo.m_userName.isEmpty()) { userInfo.m_userName = QStringLiteral("未命名会话"); }
                    userInfo.m_avatar = QIcon(":/images/defaultAvatar.png");

                    // SingleSessionDetailWidget 关闭时会自动销毁，QPointer 随后自动置空。
                    this->m_sessionDetailWidget = new SingleSessionDetailWidget(userInfo, this);
                }

                // 详情面板右边缘与聊天区标题栏对齐，并从标题栏下方弹出。
                QPoint popupPosition = this->mapToGlobal(
                    QPoint(this->width() - this->m_sessionDetailWidget->width(), this->height()));

                // 窗口靠近屏幕边缘时限制在可用显示区域内。
                QScreen *screen = QGuiApplication::screenAt(popupPosition);
                if (screen == nullptr) { screen = QGuiApplication::primaryScreen(); }
                if (screen != nullptr)
                {
                    const QRect availableGeometry = screen->availableGeometry();
                    popupPosition.setX(qBound(availableGeometry.left(), popupPosition.x(),
                                              availableGeometry.right() - this->m_sessionDetailWidget->width() + 1));
                    popupPosition.setY(qBound(availableGeometry.top(), popupPosition.y(),
                                              availableGeometry.bottom() - this->m_sessionDetailWidget->height() + 1));
                }

                this->m_sessionDetailWidget->move(popupPosition);
                this->m_sessionDetailWidget->show();
                this->m_sessionDetailWidget->raise();
                this->m_sessionDetailWidget->activateWindow();
            });
}

void RightWidgetTitle::_ToggleMaximized()
{
    if (this->window()->isMaximized()) { this->window()->showNormal(); }
    else { this->window()->showMaximized(); }
    this->_UpdateMaximizeButtonIcon();
}

void RightWidgetTitle::_ToggleAlwaysOnTop(bool checked)
{
    QWidget *topLevelWindow = this->window();
    const bool wasMaximized = topLevelWindow->isMaximized();
    topLevelWindow->setWindowFlag(Qt::WindowStaysOnTopHint, checked);
    if (wasMaximized) { topLevelWindow->showMaximized(); }
    else { topLevelWindow->show(); }
}

void RightWidgetTitle::_UpdateMaximizeButtonIcon()
{
    this->m_maximizeButton->setIcon(QIcon(this->window()->isMaximized() ? kRestoreIconPath : kMaximizeIconPath));
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
