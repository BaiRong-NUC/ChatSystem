#include <widget/user_info_widget/UserInfoWidget.h>

using namespace ChatWidget;
using namespace Log;
using namespace Model;

namespace
{
    constexpr auto kDefaultAvatarPath = ":/images/defaultAvatar.png";
    constexpr qsizetype kUserIdCharactersPerLine = 24;
    constexpr qsizetype kDefaultCharactersPerLine = 28;
    constexpr int kHoverCardShowDelayMs = 500;
    constexpr int kHoverCardHideDelayMs = 260;

    QString DisplayText(const QString &value, const QString &fallback)
    {
        return value.trimmed().isEmpty() ? fallback : value;
    }

    QString WrapHoverCardText(const QString &text, qsizetype charactersPerLine)
    {
        QStringList wrappedLines;
        const QStringList sourceLines = text.split('\n');
        for (const QString &sourceLine : sourceLines)
        {
            if (sourceLine.isEmpty())
            {
                wrappedLines.append("");
                continue;
            }
            for (qsizetype index = 0; index < sourceLine.size(); index += charactersPerLine)
            {
                wrappedLines.append(sourceLine.mid(index, charactersPerLine));
            }
        }
        return wrappedLines.join('\n');
    }
}

UserInfoWidget::UserInfoWidget(const UserInfo &userInfo, QWidget *parent) : QDialog(parent)
{
    this->m_avatarButton = new QPushButton(this);
    this->m_userName = new QLabel(this);
    this->m_userTag = new QLabel(this);
    this->m_userTagName = new QLabel(this);
    this->m_userDescription = new QLabel(this);
    this->m_userPhone = new QLabel(this);
    this->m_applyfriendButton = new QPushButton(this);
    this->m_sendMessageButton = new QPushButton(this);
    this->m_delFriendButton = new QPushButton(this);
    this->m_hoverCard = new QWidget(this, Qt::ToolTip | Qt::FramelessWindowHint);
    this->m_hoverCardTitleLabel = new QLabel(this->m_hoverCard);
    this->m_hoverCardContentLabel = new QLabel(this->m_hoverCard);
    this->m_hoverShowTimer = new QTimer(this);
    this->m_hoverHideTimer = new QTimer(this);

    this->m_userId = DisplayText(userInfo.m_userId, "未分配");
    this->m_signature = DisplayText(userInfo.m_description, "未设置");
    this->_InitUserInfoWidget(userInfo);
}

void UserInfoWidget::_InitUserInfoWidget(const UserInfo &userInfo)
{
    if (this->m_avatarButton == nullptr || this->m_userName == nullptr || this->m_userTag == nullptr ||
        this->m_userTagName == nullptr || this->m_userDescription == nullptr || this->m_userPhone == nullptr ||
        this->m_applyfriendButton == nullptr || this->m_sendMessageButton == nullptr ||
        this->m_delFriendButton == nullptr ||
        this->m_hoverCard == nullptr || this->m_hoverCardTitleLabel == nullptr ||
        this->m_hoverCardContentLabel == nullptr || this->m_hoverShowTimer == nullptr ||
        this->m_hoverHideTimer == nullptr)
    {
        LogInfo(LogLevel::ERROR, "聊天对象资料窗口初始化失败:资源指针为nullptr");
        return;
    }

    this->setWindowTitle("用户资料");
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    this->setModal(false);
    this->setFixedSize(460, 310);
    this->setObjectName("userInfoWidget");
    this->setAttribute(Qt::WA_StyledBackground, true);

    this->m_avatarButton->setIcon(userInfo.m_avatar.isNull() ? QIcon(kDefaultAvatarPath) : userInfo.m_avatar);
    this->m_avatarButton->setIconSize(QSize(78, 78));
    this->m_avatarButton->setFixedSize(82, 82);
    this->m_avatarButton->setObjectName("userInfoAvatar");
    // 禁用按钮会触发 Qt 的灰度图标效果。保持启用状态，仅关闭焦点和交互。
    this->m_avatarButton->setFocusPolicy(Qt::NoFocus);
    this->m_avatarButton->setCursor(Qt::PointingHandCursor);
    this->m_avatarButton->setAccessibleDescription("用户 ID：" + this->m_userId);
    this->m_avatarButton->installEventFilter(this);

    this->m_userName->setText(DisplayText(userInfo.m_userName, "未设置名字"));
    this->m_userName->setObjectName("userInfoName");
    this->m_userName->setTextInteractionFlags(Qt::TextSelectableByMouse);

    auto initInfoRow = [](QLabel *titleLabel, QLabel *valueLabel, const QString &title, const QString &value)
    {
        titleLabel->setText(title);
        titleLabel->setObjectName("userInfoTitle");
        titleLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        valueLabel->setText(value);
        valueLabel->setObjectName("userInfoValue");
        valueLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        valueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    };
    initInfoRow(this->m_userTag, this->m_userTagName, "昵称", DisplayText(userInfo.m_userTag, "未设置"));
    this->m_userDescription->setText(this->m_signature);
    this->m_userDescription->setObjectName("userInfoValue");
    this->m_userDescription->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    this->m_userDescription->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    this->m_userDescription->setMinimumWidth(0);
    this->m_userDescription->setTextInteractionFlags(Qt::TextSelectableByMouse);
    this->m_userDescription->installEventFilter(this);
    this->m_userPhone->setText(DisplayText(userInfo.m_phone, "未绑定"));
    this->m_userPhone->setObjectName("userInfoValue");
    this->m_userPhone->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    this->m_userPhone->setTextInteractionFlags(Qt::TextSelectableByMouse);

    auto *layout = new QGridLayout(this);
    layout->setContentsMargins(24, 20, 24, 18);
    layout->setHorizontalSpacing(12);
    layout->setVerticalSpacing(8);
    layout->setColumnMinimumWidth(0, 100);
    layout->setColumnMinimumWidth(1, 44);
    layout->setColumnStretch(2, 1);
    layout->addWidget(this->m_avatarButton, 0, 0, 3, 1, Qt::AlignTop | Qt::AlignHCenter);
    layout->addWidget(this->m_userName, 0, 1, 1, 2);
    layout->addWidget(this->m_userTag, 1, 1, Qt::AlignVCenter);
    layout->addWidget(this->m_userTagName, 1, 2, Qt::AlignVCenter);

    auto *descriptionTitle = new QLabel("签名", this);
    auto *phoneTitle = new QLabel("电话", this);
    for (QLabel *title : {descriptionTitle, phoneTitle})
    {
        title->setObjectName("userInfoTitle");
        title->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    }
    layout->addWidget(descriptionTitle, 2, 1, Qt::AlignVCenter);
    layout->addWidget(this->m_userDescription, 2, 2, Qt::AlignVCenter);

    auto *divider = new QFrame(this);
    divider->setObjectName("userInfoDivider");
    divider->setFrameShape(QFrame::HLine);
    layout->addWidget(divider, 3, 0, 1, 3);

    layout->addWidget(phoneTitle, 4, 1, Qt::AlignVCenter);
    layout->addWidget(this->m_userPhone, 4, 2, Qt::AlignVCenter);

    auto *actionDivider = new QFrame(this);
    actionDivider->setObjectName("userInfoDivider");
    actionDivider->setFrameShape(QFrame::HLine);
    layout->addWidget(actionDivider, 5, 0, 1, 3);

    auto initActionButton = [](QPushButton *button, const QString &text, const QString &role)
    {
        button->setText(text);
        button->setObjectName("userInfoActionButton");
        button->setProperty("role", role);
        button->setCursor(Qt::PointingHandCursor);
        button->setFocusPolicy(Qt::NoFocus);
        button->setMinimumWidth(88);
        button->setFixedHeight(34);
    };
    initActionButton(this->m_delFriendButton, "删除好友", "danger");
    initActionButton(this->m_applyfriendButton, "申请好友", "secondary");
    initActionButton(this->m_sendMessageButton, "发送消息", "primary");

    auto *actionLayout = new QHBoxLayout();
    actionLayout->setContentsMargins(0, 2, 0, 0);
    actionLayout->setSpacing(10);
    actionLayout->addWidget(this->m_delFriendButton);
    actionLayout->addStretch();
    actionLayout->addWidget(this->m_applyfriendButton);
    actionLayout->addWidget(this->m_sendMessageButton);
    layout->addLayout(actionLayout, 6, 0, 1, 3);

    connect(this->m_applyfriendButton, &QPushButton::clicked, this,
            [this]() { emit this->applyFriendRequested(this->m_userId); });
    connect(this->m_sendMessageButton, &QPushButton::clicked, this,
            [this]() { emit this->sendMessageRequested(this->m_userId); });
    connect(this->m_delFriendButton, &QPushButton::clicked, this,
            [this]() { emit this->deleteFriendRequested(this->m_userId); });

    this->_InitHoverCard();
    this->_UpdateSignatureDisplay();
}

void UserInfoWidget::_UpdateSignatureDisplay()
{
    if (this->m_userDescription == nullptr) { return; }

    const int availableWidth = qMax(0, this->m_userDescription->contentsRect().width() - 8);
    const QString elidedSignature =
        this->m_userDescription->fontMetrics().elidedText(this->m_signature, Qt::ElideRight, availableWidth);
    this->m_userDescription->setText(elidedSignature);
    this->m_userDescription->setProperty("elided", elidedSignature != this->m_signature);
    this->m_userDescription->setAccessibleDescription(
        elidedSignature == this->m_signature ? "用户签名" : this->m_signature);
}

void UserInfoWidget::_InitHoverCard()
{
    // 与 SelfWidget 使用完全相同的对象名，直接复用同一套悬浮信息卡样式。
    this->m_hoverCard->setObjectName("selfHoverCard");
    this->m_hoverCard->setAttribute(Qt::WA_StyledBackground, true);
    this->m_hoverCard->setAttribute(Qt::WA_ShowWithoutActivating);
    this->m_hoverCard->installEventFilter(this);

    this->m_hoverCardTitleLabel->setObjectName("selfHoverCardTitleLabel");
    this->m_hoverCardContentLabel->setObjectName("selfHoverCardContentLabel");
    this->m_hoverCardContentLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    this->m_hoverCardContentLabel->setWordWrap(true);

    auto *hoverCardLayout = new QVBoxLayout(this->m_hoverCard);
    hoverCardLayout->setContentsMargins(14, 11, 14, 12);
    hoverCardLayout->setSpacing(5);
    hoverCardLayout->addWidget(this->m_hoverCardTitleLabel);
    hoverCardLayout->addWidget(this->m_hoverCardContentLabel);

    this->m_hoverShowTimer->setSingleShot(true);
    this->m_hoverHideTimer->setSingleShot(true);
    connect(this->m_hoverShowTimer, &QTimer::timeout, this, &UserInfoWidget::_ShowHoverCard);
    connect(this->m_hoverHideTimer, &QTimer::timeout, this,
            [this]()
            {
                this->m_hoverCard->hide();
                this->m_activeHoverSource.clear();
            });
    this->m_hoverCard->hide();
}

bool UserInfoWidget::_SupportsHoverCard(QObject *source) const
{
    if (source == this->m_avatarButton) { return true; }
    return source == this->m_userDescription && this->m_userDescription->property("elided").toBool();
}

void UserInfoWidget::_ScheduleHoverCard(QObject *source)
{
    if (!this->_SupportsHoverCard(source)) { return; }

    this->m_hoverHideTimer->stop();
    if (this->m_hoverCard->isVisible() && this->m_activeHoverSource == source) { return; }

    this->m_hoverCard->hide();
    this->m_activeHoverSource.clear();
    this->m_pendingHoverSource = source;
    this->m_hoverShowTimer->stop();
    this->m_hoverShowTimer->start(kHoverCardShowDelayMs);
}

void UserInfoWidget::_ScheduleHideHoverCard()
{
    this->m_hoverShowTimer->stop();
    this->m_hoverHideTimer->stop();
    this->m_hoverHideTimer->start(kHoverCardHideDelayMs);
}

void UserInfoWidget::_ShowHoverCard()
{
    QObject *source = this->m_pendingHoverSource;
    if (!this->_SupportsHoverCard(source)) { return; }

    const bool showsUserId = source == this->m_avatarButton;
    const QString title = showsUserId ? "用户 ID" : "完整签名";
    const QString content = showsUserId ? this->m_userId : this->m_signature;
    const qsizetype charactersPerLine =
        showsUserId ? kUserIdCharactersPerLine : kDefaultCharactersPerLine;
    this->m_hoverCardContentLabel->setProperty("kind", showsUserId ? "id" : "text");
    this->m_hoverCardContentLabel->style()->unpolish(this->m_hoverCardContentLabel);
    this->m_hoverCardContentLabel->style()->polish(this->m_hoverCardContentLabel);
    this->m_hoverCardTitleLabel->setText(title);
    this->m_hoverCardContentLabel->setText(WrapHoverCardText(content, charactersPerLine));

    const QStringList lines = this->m_hoverCardContentLabel->text().split('\n');
    int widestLine = 0;
    for (const QString &line : lines)
    {
        widestLine = qMax(widestLine, this->m_hoverCardContentLabel->fontMetrics().horizontalAdvance(line));
    }
    const int cardWidth = qBound(150, widestLine + 30, 270);
    this->m_hoverCard->setFixedWidth(cardWidth);
    this->m_hoverCardContentLabel->setFixedWidth(cardWidth - 28);
    this->m_hoverCard->adjustSize();

    auto *sourceWidget = qobject_cast<QWidget *>(source);
    QPoint cardPosition = sourceWidget->mapToGlobal(QPoint(0, sourceWidget->height() + 8));
    const QRect availableGeometry = sourceWidget->screen()->availableGeometry();
    if (cardPosition.x() + this->m_hoverCard->width() > availableGeometry.right())
    {
        cardPosition.setX(availableGeometry.right() - this->m_hoverCard->width());
    }
    if (cardPosition.y() + this->m_hoverCard->height() > availableGeometry.bottom())
    {
        cardPosition.setY(sourceWidget->mapToGlobal(QPoint(0, -this->m_hoverCard->height() - 8)).y());
    }

    this->m_activeHoverSource = source;
    this->m_hoverCard->move(cardPosition);
    this->m_hoverCard->show();
    this->m_hoverCard->raise();
}

bool UserInfoWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->m_userDescription && event->type() == QEvent::Resize)
    {
        this->_UpdateSignatureDisplay();
    }

    if (event->type() == QEvent::Enter)
    {
        if (watched == this->m_hoverCard)
        {
            this->m_hoverHideTimer->stop();
        }
        else
        {
            this->_ScheduleHoverCard(watched);
        }
    }
    else if (event->type() == QEvent::Leave &&
             (watched == this->m_hoverCard || watched == this->m_activeHoverSource ||
              watched == this->m_pendingHoverSource))
    {
        this->_ScheduleHideHoverCard();
    }

    return QDialog::eventFilter(watched, event);
}
