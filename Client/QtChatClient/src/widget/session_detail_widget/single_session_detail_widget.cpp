#include <widget/session_detail_widget/single_session_detail_widget.h>

#include <QStyleOptionButton>

using namespace ChatWidget;
using namespace Log;
using namespace Model;

namespace
{
    constexpr int kPanelWidth = 460;
    constexpr int kPanelHeight = 660;
    constexpr int kMemberAreaHeight = 140;
    constexpr int kSettingRowHeight = 72;
    constexpr int kSwitchWidth = 54;
    constexpr int kSwitchHeight = 30;
    constexpr int kSwitchMargin = 3;
    constexpr int kChevronHalfHeight = 6;

    class DetailNavigationButton final : public QPushButton
    {
       public:
        explicit DetailNavigationButton(QWidget *parent = nullptr) : QPushButton(parent) {}

       protected:
        void paintEvent(QPaintEvent *event) override
        {
            QPushButton::paintEvent(event);

            QStyleOptionButton option;
            this->initStyleOption(&option);

            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(QPen(option.palette.color(QPalette::ButtonText), 2, Qt::SolidLine, Qt::RoundCap,
                                Qt::RoundJoin));

            const QPoint chevronCenter(this->width() - 18, this->height() / 2);
            painter.drawLine(chevronCenter.x() - 4, chevronCenter.y() - kChevronHalfHeight,
                             chevronCenter.x() + 2, chevronCenter.y());
            painter.drawLine(chevronCenter.x() + 2, chevronCenter.y(), chevronCenter.x() - 4,
                             chevronCenter.y() + kChevronHalfHeight);
        }
    };

    class DetailSwitch final : public QPushButton
    {
       public:
        explicit DetailSwitch(QWidget *parent = nullptr) : QPushButton(parent)
        {
            this->setCheckable(true);
            this->setChecked(false);
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
                                            : (this->underMouse() ? QColor("#38a868") : QColor("#2f8f58"));
            }
            else
            {
                trackColor = this->isDown() ? QColor("#303236")
                                            : (this->underMouse() ? QColor("#45484d") : QColor("#3a3c40"));
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
        divider->setObjectName("sessionDetailDivider");
        divider->setFrameShape(QFrame::HLine);
        divider->setFixedHeight(1);
        return divider;
    }
}  // namespace

SingleSessionDetailWidget::SingleSessionDetailWidget(const Model::UserInfo &userInfo, QWidget *parent) : QWidget(parent)
{
    this->m_avatarName = new AvatarName(userInfo.m_avatar, userInfo.m_userName, this);
    this->m_addGroup = new AddGroup(this);
    this->m_searchMessageButton = new DetailNavigationButton(this);
    this->m_messageDoNotDisturbSwitch = new DetailSwitch(this);
    this->m_pinChatSwitch = new DetailSwitch(this);
    this->m_clearHistoryButton = new QPushButton(this);

    this->_InitSingleSessionDetailWidget(userInfo);
}

void SingleSessionDetailWidget::_InitSingleSessionDetailWidget(const Model::UserInfo &userInfo)
{
    Q_UNUSED(userInfo);

    if (this->m_avatarName == nullptr || this->m_addGroup == nullptr || this->m_searchMessageButton == nullptr ||
        this->m_messageDoNotDisturbSwitch == nullptr || this->m_pinChatSwitch == nullptr ||
        this->m_clearHistoryButton == nullptr)
    {
        LogInfo(LogLevel::ERROR, "单聊会话详情窗口初始化失败:资源指针为nullptr");
        return;
    }

    this->setObjectName("singleSessionDetailWidget");
    this->setWindowTitle("会话详情");
    this->setWindowIcon(QIcon(":/images/logo.png"));
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFixedSize(kPanelWidth, kPanelHeight);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 18, 24, 24);
    mainLayout->setSpacing(0);

    // 顶部成员区：当前好友与“添加群聊”入口。
    QWidget *memberArea = new QWidget(this);
    memberArea->setObjectName("sessionDetailMemberArea");
    memberArea->setFixedHeight(kMemberAreaHeight);
    QHBoxLayout *memberLayout = new QHBoxLayout(memberArea);
    memberLayout->setContentsMargins(0, 0, 0, 10);
    memberLayout->setSpacing(0);
    memberLayout->addWidget(this->m_avatarName);
    memberLayout->addWidget(this->m_addGroup);
    memberLayout->addStretch();
    mainLayout->addWidget(memberArea);

    mainLayout->addWidget(CreateDivider(this));

    // 查找聊天内容。当前仅展示入口，不连接搜索业务。
    this->m_searchMessageButton->setObjectName("sessionDetailNavigationButton");
    this->m_searchMessageButton->setText(QStringLiteral("查找聊天内容"));
    this->m_searchMessageButton->setFixedHeight(kSettingRowHeight);
    this->m_searchMessageButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->m_searchMessageButton->setCursor(Qt::PointingHandCursor);
    this->m_searchMessageButton->setFocusPolicy(Qt::NoFocus);
    mainLayout->addWidget(this->m_searchMessageButton);

    mainLayout->addWidget(CreateDivider(this));

    auto addSwitchRow = [this, mainLayout](const QString &title, QPushButton *detailSwitch,
                                           const QString &accessibleName)
    {
        QWidget *row = new QWidget(this);
        row->setObjectName("sessionDetailSettingRow");
        row->setFixedHeight(kSettingRowHeight);

        QHBoxLayout *rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(0, 0, 0, 0);
        rowLayout->setSpacing(16);

        QLabel *titleLabel = new QLabel(title, row);
        titleLabel->setObjectName("sessionDetailSettingLabel");
        rowLayout->addWidget(titleLabel);
        rowLayout->addStretch();

        detailSwitch->setObjectName("sessionDetailSwitch");
        detailSwitch->setCursor(Qt::PointingHandCursor);
        detailSwitch->setAccessibleName(accessibleName);
        rowLayout->addWidget(detailSwitch, 0, Qt::AlignVCenter);

        mainLayout->addWidget(row);
    };

    // 开关当前只切换界面状态，不读写本地配置或服务端数据。
    addSwitchRow(QStringLiteral("消息免打扰"), this->m_messageDoNotDisturbSwitch,
                 QStringLiteral("消息免打扰"));
    addSwitchRow(QStringLiteral("置顶聊天"), this->m_pinChatSwitch, QStringLiteral("置顶聊天"));

    mainLayout->addWidget(CreateDivider(this));

    // 危险操作入口。当前不连接清理聊天记录逻辑。
    this->m_clearHistoryButton->setObjectName("clearChatHistoryButton");
    this->m_clearHistoryButton->setText(QStringLiteral("清空聊天记录"));
    this->m_clearHistoryButton->setFixedHeight(kSettingRowHeight);
    this->m_clearHistoryButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->m_clearHistoryButton->setCursor(Qt::PointingHandCursor);
    this->m_clearHistoryButton->setFocusPolicy(Qt::NoFocus);
    mainLayout->addWidget(this->m_clearHistoryButton);
    mainLayout->addStretch();
}
