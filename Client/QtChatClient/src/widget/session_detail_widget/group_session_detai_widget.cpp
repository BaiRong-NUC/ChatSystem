#include <widget/session_detail_widget/group_session_detai_widget.h>

using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr int kInitialPanelHeight = 760;
    constexpr int kMinimumPanelWidth = 340;
    constexpr int kMinimumPanelHeight = 480;
    constexpr int kMemberColumnCount = 4;
    constexpr int kSettingRowHeight = 58;
    constexpr int kInformationRowHeight = 76;
    constexpr int kSwitchWidth = 44;
    constexpr int kSwitchHeight = 24;
    constexpr int kSwitchMargin = 3;
    constexpr int kChevronHalfHeight = 6;

    class GroupNavigationButton final : public QPushButton
    {
       public:
        explicit GroupNavigationButton(QWidget *parent = nullptr) : QPushButton(parent)
        {
            this->setFlat(true);
            this->setFocusPolicy(Qt::NoFocus);
        }

       protected:
        void paintEvent(QPaintEvent *event) override
        {
            Q_UNUSED(event);

            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing, true);

            // 不调用 QPushButton 默认绘制，避免不同平台出现整块边框和渐变背景。
            if (this->underMouse())
            {
                painter.setPen(Qt::NoPen);
                painter.setBrush(this->isDown() ? QColor("#252525") : QColor("#303030"));
                painter.drawRoundedRect(this->rect().adjusted(0, 2, 0, -2), 5, 5);
            }

            const QColor foreground = this->isDown() ? QColor("#d6d6d6")
                                                       : (this->underMouse() ? QColor("#ffffff")
                                                                            : QColor("#ededed"));
            painter.setPen(foreground);
            painter.setFont(this->font());
            painter.drawText(this->rect().adjusted(0, 0, -42, 0),
                             Qt::AlignLeft | Qt::AlignVCenter, this->text());

            const QPoint center(this->width() - 16, this->height() / 2);
            painter.setPen(QPen(foreground, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawLine(center.x() - 4, center.y() - kChevronHalfHeight,
                             center.x() + 2, center.y());
            painter.drawLine(center.x() + 2, center.y(), center.x() - 4,
                             center.y() + kChevronHalfHeight);
        }
    };

    class GroupShowMoreButton final : public QPushButton
    {
       public:
        explicit GroupShowMoreButton(QWidget *parent = nullptr) : QPushButton(parent)
        {
            this->setFlat(true);
            this->setFocusPolicy(Qt::NoFocus);
        }

       protected:
        void paintEvent(QPaintEvent *event) override
        {
            Q_UNUSED(event);

            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing, true);
            if (this->underMouse())
            {
                painter.setPen(Qt::NoPen);
                painter.setBrush(this->isDown() ? QColor("#252525") : QColor("#303030"));
                painter.drawRoundedRect(this->rect().adjusted(0, 3, 0, -3), 5, 5);
            }

            const QColor foreground = this->isDown() ? QColor("#989898")
                                                       : (this->underMouse() ? QColor("#dedede")
                                                                            : QColor("#a9a9a9"));
            painter.setFont(this->font());
            painter.setPen(foreground);
            const int textWidth = painter.fontMetrics().horizontalAdvance(this->text());
            const int textLeft = (this->width() - textWidth) / 2 - 8;
            painter.drawText(QRect(textLeft, 0, textWidth, this->height()),
                             Qt::AlignCenter, this->text());

            // 在文字右侧绘制向下折线，样式与参考图中的“查看更多”一致。
            const QPoint arrowCenter(textLeft + textWidth + 16, this->height() / 2 + 1);
            painter.setPen(QPen(foreground, 1.7, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawLine(arrowCenter + QPoint(-6, -3), arrowCenter);
            painter.drawLine(arrowCenter, arrowCenter + QPoint(6, -3));
        }
    };

    class GroupDetailSwitch final : public QPushButton
    {
       public:
        explicit GroupDetailSwitch(bool checked, QWidget *parent = nullptr) : QPushButton(parent)
        {
            this->setCheckable(true);
            this->setChecked(checked);
            this->setFixedSize(kSwitchWidth, kSwitchHeight);
        }

       protected:
        void paintEvent(QPaintEvent *event) override
        {
            Q_UNUSED(event);

            QColor trackColor;
            if (this->isChecked())
            {
                trackColor = this->isDown() ? QColor("#28784b")
                                            : (this->underMouse() ? QColor("#38a868") : QColor("#07c160"));
            }
            else
            {
                trackColor = this->isDown() ? QColor("#303236")
                                            : (this->underMouse() ? QColor("#4a4c50") : QColor("#3a3c40"));
            }

            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(Qt::NoPen);
            painter.setBrush(trackColor);
            painter.drawRoundedRect(this->rect(), kSwitchHeight / 2.0, kSwitchHeight / 2.0);

            const int knobSize = kSwitchHeight - kSwitchMargin * 2;
            const int knobX = this->isChecked() ? this->width() - knobSize - kSwitchMargin : kSwitchMargin;
            painter.setBrush(QColor("#f4f4f4"));
            painter.drawEllipse(QRect(knobX, kSwitchMargin, knobSize, knobSize));
        }
    };

    QFrame *CreateDivider(QWidget *parent)
    {
        QFrame *divider = new QFrame(parent);
        divider->setObjectName("groupSessionDetailDivider");
        divider->setFrameShape(QFrame::HLine);
        divider->setFixedHeight(1);
        return divider;
    }

    QWidget *CreateInformationRow(QWidget *parent, const QString &title, const QString &value,
                                  QPointer<QLabel> &valueLabel)
    {
        QWidget *row = new QWidget(parent);
        row->setObjectName("groupSessionInformationRow");
        row->setFixedHeight(kInformationRowHeight);

        QVBoxLayout *rowLayout = new QVBoxLayout(row);
        rowLayout->setContentsMargins(0, 9, 0, 9);
        rowLayout->setSpacing(5);

        QLabel *titleLabel = new QLabel(title, row);
        titleLabel->setObjectName("groupSessionInformationTitle");
        valueLabel = new QLabel(value, row);
        valueLabel->setObjectName("groupSessionInformationValue");
        valueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        rowLayout->addWidget(titleLabel);
        rowLayout->addWidget(valueLabel);
        return row;
    }
}  // namespace

GroupSessionDetailWidget::GroupSessionDetailWidget(const QString &groupName, QWidget *parent)
    : QWidget(parent), m_groupName(groupName.trimmed().isEmpty() ? QStringLiteral("未命名群聊")
                                                                 : groupName.trimmed())
{
    this->m_scrollArea = new QScrollArea(this);
    this->m_contentWidget = new QWidget(this->m_scrollArea);
    this->m_memberSearchBox = new SearchBox(this->m_contentWidget);
    this->m_memberGridWidget = new QWidget(this->m_contentWidget);
    this->m_addGroup = new AddGroup(this->m_memberGridWidget);
    this->m_showMoreButton = new GroupShowMoreButton(this->m_contentWidget);

    this->m_searchMessageButton = new GroupNavigationButton(this->m_contentWidget);
    this->m_messageDoNotDisturbSwitch = new GroupDetailSwitch(true, this->m_contentWidget);
    this->m_foldChatSwitch = new GroupDetailSwitch(false, this->m_contentWidget);
    this->m_followMembersButton = new GroupNavigationButton(this->m_contentWidget);
    this->m_pinChatSwitch = new GroupDetailSwitch(false, this->m_contentWidget);
    this->m_saveToContactsSwitch = new GroupDetailSwitch(false, this->m_contentWidget);
    this->m_showMemberNicknameSwitch = new GroupDetailSwitch(true, this->m_contentWidget);
    this->m_clearHistoryButton = new QPushButton(this->m_contentWidget);
    this->m_exitGroupButton = new QPushButton(this->m_contentWidget);

    this->_InitGroupSessionDetailWidget();
    this->_InitSignalSlots();
}

void GroupSessionDetailWidget::_InitGroupSessionDetailWidget()
{
    if (this->m_scrollArea == nullptr || this->m_contentWidget == nullptr ||
        this->m_memberSearchBox == nullptr || this->m_memberGridWidget == nullptr ||
        this->m_addGroup == nullptr || this->m_showMoreButton == nullptr)
    {
        LogInfo(LogLevel::ERROR, "群聊详情窗口初始化失败:成员区域资源为空");
        return;
    }

    this->setObjectName("groupSessionDetailWidget");
    this->setWindowTitle(QStringLiteral("群聊详情"));
    this->setWindowIcon(QIcon(":/images/logo.png"));
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setMinimumSize(kMinimumPanelWidth, kMinimumPanelHeight);
    this->resize(PREFERRED_WIDTH, kInitialPanelHeight);

    QVBoxLayout *windowLayout = new QVBoxLayout(this);
    windowLayout->setContentsMargins(0, 0, 0, 0);
    windowLayout->setSpacing(0);

    this->m_scrollArea->setObjectName("groupSessionDetailScrollArea");
    this->m_scrollArea->setWidgetResizable(true);
    this->m_scrollArea->setFrameShape(QFrame::NoFrame);
    this->m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->m_scrollArea->verticalScrollBar()->setObjectName("groupSessionDetailScrollBar");
    this->m_contentWidget->setObjectName("groupSessionDetailContent");
    this->m_contentWidget->setAttribute(Qt::WA_StyledBackground, true);
    this->m_scrollArea->setWidget(this->m_contentWidget);
    windowLayout->addWidget(this->m_scrollArea);

    QVBoxLayout *contentLayout = new QVBoxLayout(this->m_contentWidget);
    contentLayout->setContentsMargins(20, 18, 20, 20);
    contentLayout->setSpacing(0);

    this->m_memberSearchBox->SetPlaceholderText(QStringLiteral("搜索群成员"));
    contentLayout->addWidget(this->m_memberSearchBox);
    contentLayout->addSpacing(14);

    this->m_memberGridWidget->setObjectName("groupSessionMemberGrid");
    this->m_memberGridWidget->setAttribute(Qt::WA_StyledBackground, true);
    QGridLayout *memberGridLayout = new QGridLayout(this->m_memberGridWidget);
    memberGridLayout->setContentsMargins(0, 0, 0, 0);
    memberGridLayout->setHorizontalSpacing(0);
    memberGridLayout->setVerticalSpacing(0);
    contentLayout->addWidget(this->m_memberGridWidget);

#if DEBUG_CODE
    const QIcon memberAvatar(":/images/defaultAvatar.png");
    const QStringList exampleMembers = {
        QStringLiteral("林墨"), QStringLiteral("郭立晔"), QStringLiteral("胡一鑫"),
        QStringLiteral("橘子汽水"), QStringLiteral("陈安然"), QStringLiteral("周小北"),
        QStringLiteral("王可欣"), QStringLiteral("张明远"), QStringLiteral("孙嘉树"),
        QStringLiteral("阿修木"), QStringLiteral("苹果派"), QStringLiteral("赵一然"),
        QStringLiteral("许嘉禾"), QStringLiteral("叶知秋"), QStringLiteral("苏木")};
    for (const QString &memberName : exampleMembers) { this->_AddExampleMember(memberAvatar, memberName); }
#endif
    memberGridLayout->addWidget(this->m_addGroup, this->m_memberItems.size() / kMemberColumnCount,
                                this->m_memberItems.size() % kMemberColumnCount);

    this->m_showMoreButton->setObjectName("groupSessionShowMoreButton");
    this->m_showMoreButton->setText(QStringLiteral("查看更多"));
    this->m_showMoreButton->setCursor(Qt::PointingHandCursor);
    this->m_showMoreButton->setFocusPolicy(Qt::NoFocus);
    this->m_showMoreButton->setFixedSize(150, 42);
    contentLayout->addWidget(this->m_showMoreButton, 0, Qt::AlignHCenter);
    contentLayout->addSpacing(4);
    contentLayout->addWidget(CreateDivider(this->m_contentWidget));

    contentLayout->addWidget(CreateInformationRow(this->m_contentWidget, QStringLiteral("群聊名称"),
                                                  this->m_groupName, this->m_groupNameLabel));
    // 以下群公告只展示占位信息，本阶段不提供编辑、查看详情或服务端读写。
    contentLayout->addWidget(CreateInformationRow(this->m_contentWidget, QStringLiteral("群公告"),
                                                  QStringLiteral("群主未设置"),
                                                  this->m_groupAnnouncementLabel));
    contentLayout->addWidget(CreateInformationRow(this->m_contentWidget, QStringLiteral("备注"),
                                                  QStringLiteral("群聊的备注仅自己可见"),
                                                  this->m_groupRemarkLabel));
    contentLayout->addWidget(CreateInformationRow(this->m_contentWidget, QStringLiteral("我在本群的昵称"),
#if DEBUG_CODE
                                                  QStringLiteral("115 姜柏荣"),
#else
                                                  QStringLiteral("未设置"),
#endif
                                                  this->m_groupNicknameLabel));
    contentLayout->addWidget(CreateDivider(this->m_contentWidget));

    auto addNavigationRow = [this, contentLayout](QPushButton *button, const QString &title,
                                                   const QString &accessibleName)
    {
        button->setObjectName("groupSessionNavigationButton");
        button->setText(title);
        button->setAccessibleName(accessibleName);
        button->setCursor(Qt::PointingHandCursor);
        button->setFocusPolicy(Qt::NoFocus);
        button->setFixedHeight(kSettingRowHeight);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        contentLayout->addWidget(button);
        contentLayout->addWidget(CreateDivider(this->m_contentWidget));
    };

    auto addSwitchRow = [this, contentLayout](const QString &title, QPushButton *detailSwitch,
                                               const QString &accessibleName)
    {
        QWidget *row = new QWidget(this->m_contentWidget);
        row->setObjectName("groupSessionSettingRow");
        row->setFixedHeight(kSettingRowHeight);
        QHBoxLayout *rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(0, 0, 0, 0);
        rowLayout->setSpacing(16);

        QLabel *titleLabel = new QLabel(title, row);
        titleLabel->setObjectName("groupSessionSettingLabel");
        rowLayout->addWidget(titleLabel);
        rowLayout->addStretch();

        detailSwitch->setObjectName("groupSessionSwitch");
        detailSwitch->setAccessibleName(accessibleName);
        detailSwitch->setCursor(Qt::PointingHandCursor);
        rowLayout->addWidget(detailSwitch, 0, Qt::AlignVCenter);
        contentLayout->addWidget(row);
    };

    addNavigationRow(this->m_searchMessageButton, QStringLiteral("查找聊天内容"),
                     QStringLiteral("查找群聊内容"));
    addSwitchRow(QStringLiteral("消息免打扰"), this->m_messageDoNotDisturbSwitch,
                 QStringLiteral("消息免打扰"));

    // 以下三个入口与“群公告”一样只搭建 UI；开关变化不保存，按钮不连接业务槽。
    addSwitchRow(QStringLiteral("折叠该聊天"), this->m_foldChatSwitch, QStringLiteral("折叠该聊天"));
    addNavigationRow(this->m_followMembersButton, QStringLiteral("关注的群成员"),
                     QStringLiteral("关注的群成员"));
    addSwitchRow(QStringLiteral("置顶聊天"), this->m_pinChatSwitch, QStringLiteral("置顶聊天"));
    addSwitchRow(QStringLiteral("保存到通讯录"), this->m_saveToContactsSwitch,
                 QStringLiteral("保存到通讯录"));
    addSwitchRow(QStringLiteral("显示群成员昵称"), this->m_showMemberNicknameSwitch,
                 QStringLiteral("显示群成员昵称"));
    contentLayout->addWidget(CreateDivider(this->m_contentWidget));

    this->m_clearHistoryButton->setObjectName("groupSessionDangerButton");
    this->m_clearHistoryButton->setFlat(true);
    this->m_clearHistoryButton->setFocusPolicy(Qt::NoFocus);
    this->m_clearHistoryButton->setText(QStringLiteral("清空聊天记录"));
    this->m_clearHistoryButton->setFixedHeight(kSettingRowHeight);
    contentLayout->addWidget(this->m_clearHistoryButton);
    contentLayout->addWidget(CreateDivider(this->m_contentWidget));

    this->m_exitGroupButton->setObjectName("groupSessionDangerButton");
    this->m_exitGroupButton->setFlat(true);
    this->m_exitGroupButton->setFocusPolicy(Qt::NoFocus);
    this->m_exitGroupButton->setText(QStringLiteral("退出群聊"));
    this->m_exitGroupButton->setFixedHeight(kSettingRowHeight);
    contentLayout->addWidget(this->m_exitGroupButton);
    contentLayout->addStretch();
}

void GroupSessionDetailWidget::_InitSignalSlots()
{
    if (this->m_memberSearchBox == nullptr) { return; }

    // 当前仅实现本地成员过滤；四个明确暂缓的功能不在这里连接任何业务槽。
    connect(this->m_memberSearchBox, &SearchBox::KeywordChanged, this,
            &GroupSessionDetailWidget::_FilterGroupMembers);
    connect(this->m_memberSearchBox, &SearchBox::SearchRequested, this,
            &GroupSessionDetailWidget::_FilterGroupMembers);
}

void GroupSessionDetailWidget::_AddExampleMember(const QIcon &avatar, const QString &name)
{
    if (this->m_memberGridWidget == nullptr) { return; }
    QGridLayout *memberGridLayout = qobject_cast<QGridLayout *>(this->m_memberGridWidget->layout());
    if (memberGridLayout == nullptr) { return; }

    AvatarName *memberItem = new AvatarName(avatar, name, this->m_memberGridWidget);
    const int index = this->m_memberItems.size();
    memberGridLayout->addWidget(memberItem, index / kMemberColumnCount, index % kMemberColumnCount);
    this->m_memberItems.append(memberItem);
    this->m_memberNames.append(name);
}

void GroupSessionDetailWidget::_FilterGroupMembers(const QString &keyword)
{
    if (this->m_memberGridWidget == nullptr || this->m_addGroup == nullptr) { return; }
    QGridLayout *memberGridLayout = qobject_cast<QGridLayout *>(this->m_memberGridWidget->layout());
    if (memberGridLayout == nullptr) { return; }

    const QString normalizedKeyword = keyword.trimmed();
    int visibleIndex = 0;
    for (qsizetype index = 0; index < this->m_memberItems.size(); ++index)
    {
        const QPointer<AvatarName> &memberItem = this->m_memberItems.at(index);
        if (memberItem == nullptr) { continue; }

        memberGridLayout->removeWidget(memberItem);
        const bool matched = normalizedKeyword.isEmpty() ||
                             this->m_memberNames.value(index).contains(normalizedKeyword, Qt::CaseInsensitive);
        memberItem->setVisible(matched);
        if (!matched) { continue; }

        memberGridLayout->addWidget(memberItem, visibleIndex / kMemberColumnCount,
                                    visibleIndex % kMemberColumnCount);
        ++visibleIndex;
    }

    // “添加”入口始终显示，并跟随过滤结果移动到最后一个网格位置。
    memberGridLayout->removeWidget(this->m_addGroup);
    memberGridLayout->addWidget(this->m_addGroup, visibleIndex / kMemberColumnCount,
                                visibleIndex % kMemberColumnCount);
}
