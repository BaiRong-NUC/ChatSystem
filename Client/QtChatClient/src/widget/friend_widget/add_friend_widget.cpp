#include <widget/friend_widget/add_friend_widget.h>

#include <QStyleOptionButton>

using namespace ChatWidget;

namespace
{
    constexpr int kWindowWidth = 650;
    constexpr int kWindowHeight = 780;
    constexpr int kHeaderHeight = 92;
    constexpr auto kDefaultAvatarPath = ":/images/defaultAvatar.png";

    QString DisplayText(const QString &value, const QString &fallback)
    {
        return value.trimmed().isEmpty() ? fallback : value.trimmed();
    }

    class SearchIconWidget final : public QWidget
    {
       public:
        explicit SearchIconWidget(QWidget *parent) : QWidget(parent)
        {
            this->setFixedSize(25, 25);
            this->setAttribute(Qt::WA_TransparentForMouseEvents);
        }

       protected:
        void paintEvent(QPaintEvent *event) override
        {
            Q_UNUSED(event);
            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(QPen(QColor("#a9aaad"), 2.2, Qt::SolidLine, Qt::RoundCap));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(QRectF(2.5, 2.5, 14, 14));
            painter.drawLine(QPointF(15.5, 15.5), QPointF(22.5, 22.5));
        }
    };

    class CloseButton final : public QPushButton
    {
       public:
        explicit CloseButton(QWidget *parent) : QPushButton(parent) {}

       protected:
        void paintEvent(QPaintEvent *event) override
        {
            QPushButton::paintEvent(event);
            QStyleOptionButton option;
            this->initStyleOption(&option);

            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(QPen(option.palette.color(QPalette::ButtonText), 1.7, Qt::SolidLine,
                                Qt::RoundCap));
            const QPoint center = this->rect().center();
            painter.drawLine(center + QPoint(-7, -7), center + QPoint(7, 7));
            painter.drawLine(center + QPoint(7, -7), center + QPoint(-7, 7));
        }
    };
}  // namespace

AddFriendWidget::AddFriendWidget(QWidget *parent) : QDialog(parent)
{
    this->m_closeButton = new CloseButton(this);
    this->m_searchEdit = new QLineEdit(this);
    this->m_searchButton = new QPushButton(this);
    this->m_feedbackLabel = new QLabel(this);
    this->m_resultCard = new QWidget(this);
    this->m_resultAvatar = new QPushButton(this->m_resultCard);
    this->m_resultName = new QLabel(this->m_resultCard);
    this->m_resultUserIdLabel = new QLabel(this->m_resultCard);
    this->m_resultPhoneLabel = new QLabel(this->m_resultCard);
    this->m_addButton = new QPushButton(this->m_resultCard);

    this->_InitAddFriendWidget();
    this->_InitSignalSlots();
}

void AddFriendWidget::_InitAddFriendWidget()
{
    if (this->m_closeButton == nullptr || this->m_searchEdit == nullptr ||
        this->m_searchButton == nullptr || this->m_feedbackLabel == nullptr ||
        this->m_resultCard == nullptr || this->m_resultAvatar == nullptr ||
        this->m_resultName == nullptr || this->m_resultUserIdLabel == nullptr ||
        this->m_resultPhoneLabel == nullptr || this->m_addButton == nullptr)
    {
        return;
    }

    this->setObjectName("addFriendWidget");
    this->setWindowTitle(QStringLiteral("添加朋友"));
    this->setWindowIcon(QIcon(":/images/logo.png"));
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setModal(false);
    this->setFixedSize(kWindowWidth, kWindowHeight);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 0, 30, 30);
    mainLayout->setSpacing(0);

    auto *header = new QWidget(this);
    header->setObjectName("addFriendHeader");
    header->setFixedHeight(kHeaderHeight);
    auto *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->addStretch();

    auto *titleLabel = new QLabel(QStringLiteral("添加朋友"), header);
    titleLabel->setObjectName("addFriendTitle");
    titleLabel->setAlignment(Qt::AlignCenter);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    this->m_closeButton->setObjectName("addFriendCloseButton");
    this->m_closeButton->setAccessibleName(QStringLiteral("关闭添加朋友窗口"));
    this->m_closeButton->setCursor(Qt::PointingHandCursor);
    this->m_closeButton->setFocusPolicy(Qt::NoFocus);
    this->m_closeButton->setFixedSize(44, 44);
    headerLayout->addWidget(this->m_closeButton, 0, Qt::AlignRight | Qt::AlignVCenter);
    // 让标题相对整个窗口保持视觉居中，抵消右侧关闭按钮宽度。
    headerLayout->insertSpacing(0, this->m_closeButton->width());
    mainLayout->addWidget(header);

    auto *searchRow = new QWidget(this);
    searchRow->setObjectName("addFriendSearchRow");
    auto *searchLayout = new QHBoxLayout(searchRow);
    searchLayout->setContentsMargins(0, 0, 0, 0);
    searchLayout->setSpacing(2);

    this->m_searchEdit->setObjectName("addFriendSearchEdit");
    this->m_searchEdit->setPlaceholderText(QStringLiteral("搜索手机号或者用户 ID"));
    this->m_searchEdit->setAccessibleName(QStringLiteral("手机号或用户 ID"));
    this->m_searchEdit->setClearButtonEnabled(true);
    this->m_searchEdit->setFixedHeight(64);
    auto *searchIcon = new SearchIconWidget(this->m_searchEdit);
    searchIcon->move(18, 19);
    searchIcon->show();
    searchLayout->addWidget(this->m_searchEdit, 1);

    this->m_searchButton->setObjectName("addFriendSearchButton");
    this->m_searchButton->setText(QStringLiteral("搜索"));
    this->m_searchButton->setAccessibleName(QStringLiteral("搜索用户"));
    this->m_searchButton->setCursor(Qt::PointingHandCursor);
    this->m_searchButton->setFixedSize(120, 64);
    searchLayout->addWidget(this->m_searchButton);
    mainLayout->addWidget(searchRow);

    this->m_feedbackLabel->setObjectName("addFriendFeedbackLabel");
    this->m_feedbackLabel->setAlignment(Qt::AlignCenter);
    this->m_feedbackLabel->setWordWrap(true);
    this->m_feedbackLabel->setMinimumHeight(72);
    this->m_feedbackLabel->hide();
    mainLayout->addWidget(this->m_feedbackLabel);

    this->m_resultCard->setObjectName("addFriendResultCard");
    this->m_resultCard->setAttribute(Qt::WA_StyledBackground, true);
    this->m_resultCard->setFixedHeight(116);
    auto *resultLayout = new QHBoxLayout(this->m_resultCard);
    resultLayout->setContentsMargins(18, 16, 18, 16);
    resultLayout->setSpacing(16);

    this->m_resultAvatar->setObjectName("addFriendResultAvatar");
    this->m_resultAvatar->setFocusPolicy(Qt::NoFocus);
    this->m_resultAvatar->setFixedSize(72, 72);
    this->m_resultAvatar->setIconSize(QSize(66, 66));
    resultLayout->addWidget(this->m_resultAvatar, 0, Qt::AlignVCenter);

    auto *informationLayout = new QVBoxLayout();
    informationLayout->setContentsMargins(0, 2, 0, 2);
    informationLayout->setSpacing(4);
    this->m_resultName->setObjectName("addFriendResultName");
    this->m_resultUserIdLabel->setObjectName("addFriendResultSecondary");
    this->m_resultPhoneLabel->setObjectName("addFriendResultSecondary");
    informationLayout->addWidget(this->m_resultName);
    informationLayout->addWidget(this->m_resultUserIdLabel);
    informationLayout->addWidget(this->m_resultPhoneLabel);
    resultLayout->addLayout(informationLayout, 1);

    this->m_addButton->setObjectName("addFriendApplyButton");
    this->m_addButton->setText(QStringLiteral("添加"));
    this->m_addButton->setCursor(Qt::PointingHandCursor);
    this->m_addButton->setFixedSize(82, 38);
    resultLayout->addWidget(this->m_addButton, 0, Qt::AlignVCenter);

    this->m_resultCard->hide();
    mainLayout->addWidget(this->m_resultCard);
    mainLayout->addStretch();

    this->m_searchEdit->setFocus(Qt::OtherFocusReason);
}

void AddFriendWidget::_InitSignalSlots()
{
    connect(this->m_closeButton, &QPushButton::clicked, this, &QDialog::close);
    connect(this->m_searchButton, &QPushButton::clicked, this, &AddFriendWidget::_SubmitSearch);
    connect(this->m_searchEdit, &QLineEdit::returnPressed, this, &AddFriendWidget::_SubmitSearch);
    connect(this->m_addButton, &QPushButton::clicked, this,
            [this]()
            {
                if (this->m_resultUserId.isEmpty()) { return; }
                this->setFriendRequestSubmitting(true);
                emit this->addFriendRequested(this->m_resultUserId);
            });
}

void AddFriendWidget::_SubmitSearch()
{
    const QString keyword = this->m_searchEdit->text().trimmed();
    if (keyword.isEmpty())
    {
        this->m_resultCard->hide();
        this->_ShowFeedback(QStringLiteral("请输入手机号或用户 ID"), QStringLiteral("error"));
        this->m_searchEdit->setFocus(Qt::OtherFocusReason);
        return;
    }

    this->m_lastSearchKeyword = keyword;
    this->m_resultUserId.clear();
    this->m_resultCard->hide();
    this->setSearching(true);
    emit this->searchUserRequested(keyword);
}

void AddFriendWidget::_SetSearchControlsEnabled(bool enabled)
{
    this->m_searchEdit->setEnabled(enabled);
    this->m_searchButton->setEnabled(enabled);
    this->m_searchButton->setText(enabled ? QStringLiteral("搜索") : QStringLiteral("搜索中..."));
}

void AddFriendWidget::_ShowFeedback(const QString &message, const QString &status)
{
    this->m_feedbackLabel->setText(message);
    this->m_feedbackLabel->setProperty("status", status);
    this->m_feedbackLabel->style()->unpolish(this->m_feedbackLabel);
    this->m_feedbackLabel->style()->polish(this->m_feedbackLabel);
    this->m_feedbackLabel->setVisible(!message.isEmpty());
}

void AddFriendWidget::_UpdateResult(const Model::UserInfo &userInfo)
{
    this->m_resultUserId = userInfo.m_userId.trimmed();
    this->m_resultAvatar->setIcon(userInfo.m_avatar.isNull() ? QIcon(kDefaultAvatarPath) : userInfo.m_avatar);
    this->m_resultName->setText(DisplayText(userInfo.m_userName, DisplayText(userInfo.m_userTag, "未设置名字")));
    this->m_resultUserIdLabel->setText(QStringLiteral("用户 ID：%1").arg(DisplayText(userInfo.m_userId, "未分配")));
    this->m_resultPhoneLabel->setText(QStringLiteral("手机号：%1").arg(DisplayText(userInfo.m_phone, "未绑定")));
    this->m_addButton->setEnabled(!this->m_resultUserId.isEmpty());
    this->m_addButton->setText(this->m_resultUserId.isEmpty() ? QStringLiteral("不可添加") : QStringLiteral("添加"));
}

void AddFriendWidget::setSearching(bool searching)
{
    this->_SetSearchControlsEnabled(!searching);
    this->m_resultCard->setVisible(!searching && !this->m_resultUserId.isEmpty());
    this->_ShowFeedback(searching ? QStringLiteral("正在搜索用户...") : QString(), QStringLiteral("loading"));
}

void AddFriendWidget::showSearchResult(const Model::UserInfo &userInfo)
{
    this->_SetSearchControlsEnabled(true);
    this->_UpdateResult(userInfo);
    this->_ShowFeedback({}, {});
    this->m_resultCard->show();
}

void AddFriendWidget::showNoSearchResult(const QString &keyword)
{
    this->_SetSearchControlsEnabled(true);
    this->m_resultUserId.clear();
    this->m_resultCard->hide();
    const QString displayedKeyword = keyword.trimmed().isEmpty() ? this->m_lastSearchKeyword : keyword.trimmed();
    this->_ShowFeedback(QStringLiteral("未找到“%1”对应的用户").arg(displayedKeyword), QStringLiteral("empty"));
}

void AddFriendWidget::showSearchError(const QString &message)
{
    this->_SetSearchControlsEnabled(true);
    this->m_resultUserId.clear();
    this->m_resultCard->hide();
    this->_ShowFeedback(message.trimmed().isEmpty() ? QStringLiteral("搜索失败，请稍后重试") : message,
                        QStringLiteral("error"));
}

void AddFriendWidget::setFriendRequestSubmitting(bool submitting)
{
    this->m_addButton->setEnabled(!submitting && !this->m_resultUserId.isEmpty());
    this->m_addButton->setText(submitting ? QStringLiteral("发送中...") : QStringLiteral("添加"));
}

void AddFriendWidget::showFriendRequestResult(bool succeeded, const QString &message)
{
    this->m_addButton->setEnabled(!succeeded && !this->m_resultUserId.isEmpty());
    this->m_addButton->setText(succeeded ? QStringLiteral("已发送") : QStringLiteral("添加"));
    const QString defaultMessage = succeeded ? QStringLiteral("好友申请已发送") : QStringLiteral("好友申请发送失败");
    this->_ShowFeedback(message.trimmed().isEmpty() ? defaultMessage : message,
                        succeeded ? QStringLiteral("success") : QStringLiteral("error"));
}

void AddFriendWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->position().y() <= kHeaderHeight)
    {
        this->m_dragging = true;
        this->m_dragOffset = event->globalPosition().toPoint() - this->frameGeometry().topLeft();
        event->accept();
        return;
    }
    QDialog::mousePressEvent(event);
}

void AddFriendWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (this->m_dragging && event->buttons().testFlag(Qt::LeftButton))
    {
        this->move(event->globalPosition().toPoint() - this->m_dragOffset);
        event->accept();
        return;
    }
    QDialog::mouseMoveEvent(event);
}

void AddFriendWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) { this->m_dragging = false; }
    QDialog::mouseReleaseEvent(event);
}
