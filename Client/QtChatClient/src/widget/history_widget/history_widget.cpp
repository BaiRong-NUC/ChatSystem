#include <widget/history_widget/history_widget.h>

#include <QCalendarWidget>
#include <QGuiApplication>

using namespace ChatWidget;

namespace
{
    constexpr int kInitialWindowWidth = 760;
    constexpr int kInitialWindowHeight = 640;
    constexpr int kMinimumWindowWidth = 600;
    constexpr int kMinimumWindowHeight = 460;
    constexpr int kTitleBarHeight = 58;
    constexpr int kAvatarSize = 46;
    constexpr auto kMinimizeIconPath = ":/images/icons/tabler/minus.svg";
    constexpr auto kCloseIconPath = ":/images/icons/tabler/x.svg";

    // QDateEdit 不能保存无效日期，因此使用它允许的最小日期表示“不限制日期”。
    const QDate kAllDateSentinel(2000, 1, 1);

    class ChatHistoryTitleBar final : public QWidget
    {
       public:
        explicit ChatHistoryTitleBar(QWidget *parent = nullptr) : QWidget(parent) {}

       protected:
        void mousePressEvent(QMouseEvent *event) override
        {
            if (event->button() == Qt::LeftButton && this->window()->windowHandle() != nullptr)
            {
                this->window()->windowHandle()->startSystemMove();
            }
            QWidget::mousePressEvent(event);
        }
    };

    /**
     * 单条聊天记录展示控件。
     *
     * 记录控件只负责头像、发送者、时间和文本摘要的排版，不保存业务状态。
     * 创建时指定结果容器为父对象，销毁由Qt对象树统一负责。
     */
    class ChatHistoryItem final : public QWidget
    {
       public:
        explicit ChatHistoryItem(const ChatHistoryRecord &record, QWidget *parent = nullptr)
            : QWidget(parent)
        {
            this->setObjectName("chatHistoryItem");
            this->setAttribute(Qt::WA_StyledBackground, true);
            this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

            QHBoxLayout *itemLayout = new QHBoxLayout(this);
            itemLayout->setContentsMargins(14, 13, 14, 13);
            itemLayout->setSpacing(14);

            QLabel *avatarLabel = new QLabel(this);
            avatarLabel->setObjectName("chatHistoryAvatar");
            avatarLabel->setFixedSize(kAvatarSize, kAvatarSize);
            avatarLabel->setAlignment(Qt::AlignCenter);
            QPixmap avatarPixmap = record.m_senderAvatar.pixmap(kAvatarSize, kAvatarSize);
            avatarLabel->setPixmap(avatarPixmap);
            itemLayout->addWidget(avatarLabel, 0, Qt::AlignTop);

            QVBoxLayout *contentLayout = new QVBoxLayout();
            contentLayout->setContentsMargins(0, 0, 0, 0);
            contentLayout->setSpacing(6);

            QHBoxLayout *informationLayout = new QHBoxLayout();
            informationLayout->setContentsMargins(0, 0, 0, 0);
            informationLayout->setSpacing(12);

            QLabel *senderLabel = new QLabel(record.m_senderName, this);
            senderLabel->setObjectName("chatHistorySender");
            senderLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
            informationLayout->addWidget(senderLabel);
            informationLayout->addStretch();

            QLabel *timeLabel = new QLabel(record.m_timestamp.toString("yyyy-MM-dd HH:mm"), this);
            timeLabel->setObjectName("chatHistoryTime");
            informationLayout->addWidget(timeLabel);
            contentLayout->addLayout(informationLayout);

            QLabel *contentLabel = new QLabel(record.m_content, this);
            contentLabel->setObjectName("chatHistoryContent");
            contentLabel->setWordWrap(true);
            contentLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
            contentLayout->addWidget(contentLabel);
            itemLayout->addLayout(contentLayout, 1);
        }
    };
}  // namespace

HistoryWidget::HistoryWidget(QWidget *parent)
    : HistoryWidget(QStringLiteral("未命名会话"), parent)
{
}

HistoryWidget::HistoryWidget(const QString &sessionName, QWidget *parent)
    : QWidget(parent),
      m_sessionName(sessionName.trimmed().isEmpty() ? QStringLiteral("未命名会话")
                                                     : sessionName.trimmed())
{
    this->m_titleBar = new ChatHistoryTitleBar(this);
    this->m_titleLabel = new QLabel(this->m_titleBar);
    this->m_minimizeButton = new QPushButton(this->m_titleBar);
    this->m_closeButton = new QPushButton(this->m_titleBar);
    this->m_searchBox = new SearchBox(this);
    this->m_dateEdit = new QDateEdit(this);
    this->m_allDateButton = new QPushButton(this);
    this->m_resultScrollArea = new AutoHideScrollArea(this);
    this->m_resultContainer = new QWidget(this->m_resultScrollArea);
    this->m_emptyLabel = new QLabel(this->m_resultContainer);

    this->_InitHistoryWidget();
    this->_InitSignalSlots();

#if DEBUG_CODE
    // 调试记录仅用于验证日期、关键字组合查询和滚动效果；正式构建不会生成这些数据。
    const QIcon exampleAvatar(":/images/defaultAvatar.png");
    const QDateTime now = QDateTime::currentDateTime();
    const QList<ChatHistoryRecord> exampleRecords = {
        {QStringLiteral("好友1"), QStringLiteral("项目公共组件的样式已经调整好了。"),
         now.addSecs(-300), exampleAvatar},
        {QStringLiteral("我"), QStringLiteral("好的，我再检查一下聊天记录窗口。"),
         now.addSecs(-180), exampleAvatar},
        {QStringLiteral("好友1"), QStringLiteral("昨天讨论的日期查询也需要保留。"),
         now.addDays(-1).addSecs(-600), exampleAvatar},
        {QStringLiteral("系统消息"), QStringLiteral("这是一条用于测试关键字过滤的历史记录。"),
         now.addDays(-7), exampleAvatar},
        {QStringLiteral("我"), QStringLiteral("聊天记录只按照日期和关键字查询。"),
         now.addDays(-14), exampleAvatar},
    };
    this->SetHistoryRecords(exampleRecords);
#endif
}

void HistoryWidget::_InitHistoryWidget()
{
    if (this->m_titleBar == nullptr || this->m_titleLabel == nullptr ||
        this->m_minimizeButton == nullptr || this->m_closeButton == nullptr ||
        this->m_searchBox == nullptr || this->m_dateEdit == nullptr ||
        this->m_allDateButton == nullptr || this->m_resultScrollArea == nullptr ||
        this->m_resultContainer == nullptr || this->m_emptyLabel == nullptr)
    {
        return;
    }

    this->setObjectName("chatHistoryWidget");
    this->setWindowTitle(QStringLiteral("聊天记录"));
    this->setWindowIcon(QIcon(":/images/logo.png"));
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setMinimumSize(kMinimumWindowWidth, kMinimumWindowHeight);
    this->resize(kInitialWindowWidth, kInitialWindowHeight);

    QVBoxLayout *windowLayout = new QVBoxLayout(this);
    windowLayout->setContentsMargins(1, 1, 1, 1);
    windowLayout->setSpacing(0);

    this->m_titleBar->setObjectName("chatHistoryTitleBar");
    this->m_titleBar->setFixedHeight(kTitleBarHeight);
    this->m_titleBar->setAttribute(Qt::WA_StyledBackground, true);
    QHBoxLayout *titleLayout = new QHBoxLayout(this->m_titleBar);
    titleLayout->setContentsMargins(22, 0, 12, 0);
    titleLayout->setSpacing(0);
    titleLayout->addStretch();

    this->m_titleLabel->setObjectName("chatHistoryTitleLabel");
    this->m_titleLabel->setAlignment(Qt::AlignCenter);
    titleLayout->addWidget(this->m_titleLabel);
    titleLayout->addStretch();

    const QList<QPushButton *> windowButtons = {this->m_minimizeButton, this->m_closeButton};
    for (QPushButton *button : windowButtons)
    {
        button->setObjectName("chatHistoryWindowButton");
        button->setProperty("variant", "icon");
        button->setFixedSize(38, 30);
        button->setIconSize(QSize(16, 16));
        button->setCursor(Qt::PointingHandCursor);
        titleLayout->addWidget(button);
    }
    this->m_minimizeButton->setIcon(QIcon(kMinimizeIconPath));
    this->m_minimizeButton->setAccessibleName(QStringLiteral("最小化聊天记录窗口"));
    this->m_closeButton->setObjectName("chatHistoryCloseButton");
    this->m_closeButton->setIcon(QIcon(kCloseIconPath));
    this->m_closeButton->setAccessibleName(QStringLiteral("关闭聊天记录窗口"));
    windowLayout->addWidget(this->m_titleBar);

    QWidget *filterArea = new QWidget(this);
    filterArea->setObjectName("chatHistoryFilterArea");
    QHBoxLayout *filterLayout = new QHBoxLayout(filterArea);
    filterLayout->setContentsMargins(28, 20, 28, 18);
    filterLayout->setSpacing(12);

    this->m_searchBox->SetPlaceholderText(QStringLiteral("搜索聊天记录"));
    filterLayout->addWidget(this->m_searchBox, 1);

    this->m_dateEdit->setObjectName("chatHistoryDateEdit");
    this->m_dateEdit->setCalendarPopup(true);
    this->m_dateEdit->setDisplayFormat("yyyy-MM-dd");
    this->m_dateEdit->setMinimumDate(kAllDateSentinel);
    this->m_dateEdit->setMaximumDate(QDate::currentDate());
    this->m_dateEdit->setSpecialValueText(QStringLiteral("全部日期"));
    this->m_dateEdit->setDate(kAllDateSentinel);
    this->m_dateEdit->setFixedSize(150, 46);
    this->m_dateEdit->setAccessibleName(QStringLiteral("聊天记录日期"));
    filterLayout->addWidget(this->m_dateEdit);

    this->m_allDateButton->setObjectName("chatHistoryAllDateButton");
    this->m_allDateButton->setText(QStringLiteral("全部"));
    this->m_allDateButton->setFixedSize(58, 46);
    this->m_allDateButton->setCursor(Qt::PointingHandCursor);
    this->m_allDateButton->setAccessibleName(QStringLiteral("查询全部日期"));
    filterLayout->addWidget(this->m_allDateButton);
    windowLayout->addWidget(filterArea);

    this->m_resultScrollArea->setObjectName("chatHistoryResultScrollArea");
    this->m_resultScrollArea->setWidgetResizable(true);
    this->m_resultScrollArea->setFrameShape(QFrame::NoFrame);
    this->m_resultContainer->setObjectName("chatHistoryResultContainer");
    this->m_resultContainer->setAttribute(Qt::WA_StyledBackground, true);
    this->m_resultLayout = new QVBoxLayout(this->m_resultContainer);
    this->m_resultLayout->setContentsMargins(28, 8, 28, 22);
    this->m_resultLayout->setSpacing(8);
    this->m_resultLayout->setAlignment(Qt::AlignTop);

    this->m_emptyLabel->setObjectName("chatHistoryEmptyLabel");
    this->m_emptyLabel->setText(QStringLiteral("没有符合条件的聊天记录"));
    this->m_emptyLabel->setAlignment(Qt::AlignCenter);
    this->m_emptyLabel->setFixedHeight(120);
    this->m_resultScrollArea->setWidget(this->m_resultContainer);
    windowLayout->addWidget(this->m_resultScrollArea, 1);

    this->_RefreshHistoryResults();
}

void HistoryWidget::_InitSignalSlots()
{
    if (this->m_searchBox == nullptr || this->m_dateEdit == nullptr ||
        this->m_allDateButton == nullptr || this->m_minimizeButton == nullptr ||
        this->m_closeButton == nullptr)
    {
        return;
    }

    connect(this->m_searchBox, &SearchBox::KeywordChanged, this,
            [this](const QString &)
            {
                this->_RefreshHistoryResults();
                emit this->HistoryQueryRequested(this->m_searchBox->m_searchEdit->text().trimmed(),
                                                 this->_SelectedDate());
            });
    connect(this->m_searchBox, &SearchBox::SearchRequested, this,
            [this](const QString &keyword)
            {
                this->_RefreshHistoryResults();
                emit this->HistoryQueryRequested(keyword.trimmed(), this->_SelectedDate());
            });
    connect(this->m_dateEdit, &QDateEdit::dateChanged, this,
            [this](const QDate &)
            {
                this->_RefreshHistoryResults();
                emit this->HistoryQueryRequested(this->m_searchBox->m_searchEdit->text().trimmed(),
                                                 this->_SelectedDate());
            });
    connect(this->m_allDateButton, &QPushButton::clicked, this,
            [this]() { this->m_dateEdit->setDate(kAllDateSentinel); });
    connect(this->m_minimizeButton, &QPushButton::clicked, this,
            [this]() { this->showMinimized(); });
    connect(this->m_closeButton, &QPushButton::clicked, this, &QWidget::close);
}

QDate HistoryWidget::_SelectedDate() const
{
    if (this->m_dateEdit == nullptr || this->m_dateEdit->date() == kAllDateSentinel)
    {
        return {};
    }
    return this->m_dateEdit->date();
}

void HistoryWidget::_ClearResultItems()
{
    if (this->m_resultLayout == nullptr) { return; }

    while (auto item = std::unique_ptr<QLayoutItem>(this->m_resultLayout->takeAt(0)))
    {
        // 立即销毁旧结果，刷新方法返回后界面和内存中的记录控件保持一致。
        std::unique_ptr<QWidget> widget(item->widget());
    }
}

void HistoryWidget::_RefreshHistoryResults()
{
    if (this->m_resultLayout == nullptr || this->m_searchBox == nullptr ||
        this->m_searchBox->m_searchEdit == nullptr)
    {
        return;
    }

    this->_ClearResultItems();
    const QString keyword = this->m_searchBox->m_searchEdit->text().trimmed();
    const QDate selectedDate = this->_SelectedDate();
    int matchedCount = 0;

    for (const ChatHistoryRecord &record : this->m_historyRecords)
    {
        const bool dateMatched = !selectedDate.isValid() || record.m_timestamp.date() == selectedDate;
        const bool keywordMatched = keyword.isEmpty() ||
                                    record.m_senderName.contains(keyword, Qt::CaseInsensitive) ||
                                    record.m_content.contains(keyword, Qt::CaseInsensitive);
        if (!dateMatched || !keywordMatched) { continue; }

        this->m_resultLayout->addWidget(new ChatHistoryItem(record, this->m_resultContainer));
        ++matchedCount;
    }

    if (matchedCount == 0)
    {
        // emptyLabel在上一次清空时被删除，因此每次空结果都创建一个新的父对象明确的标签。
        this->m_emptyLabel = new QLabel(QStringLiteral("没有符合条件的聊天记录"),
                                        this->m_resultContainer);
        this->m_emptyLabel->setObjectName("chatHistoryEmptyLabel");
        this->m_emptyLabel->setAlignment(Qt::AlignCenter);
        this->m_emptyLabel->setFixedHeight(120);
        this->m_resultLayout->addWidget(this->m_emptyLabel);
    }

    this->m_titleLabel->setText(QStringLiteral("“%1”的聊天记录（%2）")
                                    .arg(this->m_sessionName)
                                    .arg(matchedCount));
    this->m_resultLayout->addStretch();
    this->m_resultScrollArea->RefreshScrollBar();
}

void HistoryWidget::AddHistoryRecord(const ChatHistoryRecord &record)
{
    this->m_historyRecords.append(record);
    this->_RefreshHistoryResults();
}

void HistoryWidget::SetHistoryRecords(const QList<ChatHistoryRecord> &records)
{
    this->m_historyRecords = records;
    this->_RefreshHistoryResults();
}

void HistoryWidget::ClearHistoryRecords()
{
    this->m_historyRecords.clear();
    this->_RefreshHistoryResults();
}

void HistoryWidget::ShowCentered(QWidget *anchorWindow)
{
    QWidget *anchor = anchorWindow == nullptr ? this->parentWidget() : anchorWindow;
    QPoint popupPosition;
    if (anchor != nullptr)
    {
        popupPosition = anchor->mapToGlobal(anchor->rect().center()) -
                        QPoint(this->width() / 2, this->height() / 2);
    }

    // 多屏和低分辨率环境下，把完整窗口限制在当前屏幕可用区域内。
    QScreen *screen = QGuiApplication::screenAt(popupPosition);
    if (screen == nullptr) { screen = QGuiApplication::primaryScreen(); }
    if (screen != nullptr)
    {
        const QRect availableGeometry = screen->availableGeometry();
        const int maximumX = qMax(availableGeometry.left(),
                                  availableGeometry.right() - this->width() + 1);
        const int maximumY = qMax(availableGeometry.top(),
                                  availableGeometry.bottom() - this->height() + 1);
        popupPosition.setX(qBound(availableGeometry.left(), popupPosition.x(), maximumX));
        popupPosition.setY(qBound(availableGeometry.top(), popupPosition.y(), maximumY));
    }

    this->move(popupPosition);
    this->show();
    this->raise();
    this->activateWindow();
    this->m_searchBox->SetSearchFocus();
}
