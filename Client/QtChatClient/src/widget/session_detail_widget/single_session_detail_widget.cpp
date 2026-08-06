#include <widget/session_detail_widget/single_session_detail_widget.h>

#include <QGuiApplication>
#include <QStyleOptionButton>

using namespace ChatWidget;
using namespace Log;
using namespace Model;

namespace
{
    constexpr int kInitialPanelHeight = 660;
    constexpr int kMinimumPanelWidth = 320;
    constexpr int kMinimumPanelHeight = 420;
    constexpr int kMemberAreaHeight = 110;
    constexpr int kSettingRowHeight = 58;
    constexpr int kSwitchWidth = 44;
    constexpr int kSwitchHeight = 24;
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

SingleSessionDetailWidget::SingleSessionDetailWidget(const Model::UserInfo &userInfo, QWidget *parent)
    : QWidget(parent), m_sessionUserInfo(userInfo)
{
    this->m_avatarName = new AvatarName(userInfo.m_avatar, userInfo.m_userName, this);
    this->m_addGroup = new AddGroup(this);
    this->m_searchMessageButton = new DetailNavigationButton(this);
    this->m_messageDoNotDisturbSwitch = new DetailSwitch(this);
    this->m_pinChatSwitch = new DetailSwitch(this);
    this->m_clearHistoryButton = new QPushButton(this);

    this->_InitSingleSessionDetailWidget(userInfo);
    this->_InitSignalSlots();
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
    // Qt::Popup 在截图工具抢占焦点时会自动关闭，Tool 窗口可以在失焦后继续显示。
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setMinimumSize(kMinimumPanelWidth, kMinimumPanelHeight);
    this->resize(PREFERRED_WIDTH, kInitialPanelHeight);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 14, 20, 20);
    mainLayout->setSpacing(0);

    // 顶部成员区：当前好友与“添加群聊”入口。
    QWidget *memberArea = new QWidget(this);
    memberArea->setObjectName("sessionDetailMemberArea");
    memberArea->setFixedHeight(kMemberAreaHeight);
    QHBoxLayout *memberLayout = new QHBoxLayout(memberArea);
    memberLayout->setContentsMargins(0, 0, 0, 8);
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

void SingleSessionDetailWidget::_InitSignalSlots()
{
    if (this->m_addGroup == nullptr || this->m_addGroup->m_addButton == nullptr) { return; }

    // AddGroup 是可复用展示控件；具体打开哪个窗口由当前会话详情负责。
    connect(this->m_addGroup->m_addButton, &QPushButton::clicked, this,
            &SingleSessionDetailWidget::_OpenChooseFriendWidget);
}

void SingleSessionDetailWidget::_OpenChooseFriendWidget()
{
    // 每次点击都销毁旧窗口并重新创建，确保搜索词、勾选项和右侧成员列表完全复位。
    if (this->m_chooseFriendWidget != nullptr)
    {
        this->m_chooseFriendWidget->hide();
        this->m_chooseFriendWidget->deleteLater();
        this->m_chooseFriendWidget = nullptr;
    }

    // 指定 this 为父对象，详情窗口销毁时选择窗口也会由 Qt 对象树安全释放。
    this->m_chooseFriendWidget = new ChooseFriendWidget(this);

    // 从单聊创建群聊时，当前会话好友应默认出现在已选成员列表中。
    QString friendName = this->m_sessionUserInfo.m_userName.trimmed();
    if (friendName.isEmpty()) { friendName = this->m_sessionUserInfo.m_userTag.trimmed(); }
    if (friendName.isEmpty()) { friendName = QStringLiteral("未命名好友"); }
    this->m_chooseFriendWidget->AddSelectedFriend(this->m_sessionUserInfo.m_avatar, friendName);

#if DEBUG_CODE
    // 调试联系人仅用于展示和验证搜索、滚动、勾选及取消选择效果。
    // 发布前将 public.h 中的 DEBUG_CODE 关闭后，这些示例不会进入正式好友列表。
    const QIcon exampleAvatar(":/images/defaultAvatar.png");
    this->m_chooseFriendWidget->AddFriend(exampleAvatar, QStringLiteral("安然"));
    this->m_chooseFriendWidget->AddFriend(exampleAvatar, QStringLiteral("陈小雨"));
    this->m_chooseFriendWidget->AddFriend(exampleAvatar, QStringLiteral("林晓峰"));
    this->m_chooseFriendWidget->AddFriend(exampleAvatar, QStringLiteral("王可欣"));
    this->m_chooseFriendWidget->AddFriend(exampleAvatar, QStringLiteral("张明远"));
    this->m_chooseFriendWidget->AddFriend(exampleAvatar, QStringLiteral("周小北"));
#endif

    connect(this->m_chooseFriendWidget, &ChooseFriendWidget::ConfirmSelectedFriends, this,
            &SingleSessionDetailWidget::CreateGroupRequested);

    // 以主窗口为锚点居中，而不是以右侧窄详情栏居中，避免弹窗大面积超出屏幕。
    QWidget *anchorWindow = this;
    if (this->parentWidget() != nullptr && this->parentWidget()->window() != nullptr)
    {
        anchorWindow = this->parentWidget()->window();
    }
    QPoint popupPosition = anchorWindow->mapToGlobal(anchorWindow->rect().center()) -
                           QPoint(this->m_chooseFriendWidget->width() / 2,
                                  this->m_chooseFriendWidget->height() / 2);

    // 多屏环境下把完整弹窗限制在当前屏幕可用区域内。
    QScreen *screen = QGuiApplication::screenAt(popupPosition);
    if (screen == nullptr) { screen = QGuiApplication::primaryScreen(); }
    if (screen != nullptr)
    {
        const QRect availableGeometry = screen->availableGeometry();
        // 当弹窗尺寸大于可用屏幕时，右/下边界可能小于左/上边界；先钳制最大值，
        // 避免 qBound 收到反向区间并在低分辨率环境触发断言。
        const int maximumX = qMax(availableGeometry.left(),
                                  availableGeometry.right() - this->m_chooseFriendWidget->width() + 1);
        const int maximumY = qMax(availableGeometry.top(),
                                  availableGeometry.bottom() - this->m_chooseFriendWidget->height() + 1);
        popupPosition.setX(qBound(availableGeometry.left(), popupPosition.x(), maximumX));
        popupPosition.setY(qBound(availableGeometry.top(), popupPosition.y(), maximumY));
    }

    this->m_chooseFriendWidget->move(popupPosition);
    this->m_chooseFriendWidget->show();
    this->m_chooseFriendWidget->raise();
    this->m_chooseFriendWidget->activateWindow();
}
