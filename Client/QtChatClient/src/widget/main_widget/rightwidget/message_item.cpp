#include <widget/main_widget/rightwidget/message_item.h>

using namespace ChatWidget;
using namespace Log;

namespace
{
    QString DisplaySenderName(const Model::UserInfo &sender)
    {
        if (!sender.m_userTag.trimmed().isEmpty()) { return sender.m_userTag; }
        if (!sender.m_userName.trimmed().isEmpty()) { return sender.m_userName; }
        return "未命名用户";
    }

    QString DisplayMessageTime(const QString &value)
    {
        QDateTime timestamp = QDateTime::fromString(value, "yyyy-MM-dd HH:mm:ss");
        if (!timestamp.isValid()) { timestamp = QDateTime::fromString(value, Qt::ISODate); }
        if (!timestamp.isValid()) { return value.trimmed().isEmpty() ? "--:--" : value; }

        const QDate today = QDate::currentDate();
        if (timestamp.date() == today) { return timestamp.toString("HH:mm"); }
        if (timestamp.date() == today.addDays(-1)) { return timestamp.toString("昨天 HH:mm"); }
        if (timestamp.date().year() == today.year()) { return timestamp.toString("M月d日 HH:mm"); }
        return timestamp.toString("yyyy年M月d日 HH:mm");
    }
}  // namespace

MessageItem::MessageItem(QWidget *parent, const Model::Message &data, bool isLeft, const QFont &textFont)
    : QWidget(parent), m_isLeft(isLeft)
{
    // 资源初始化
    this->m_avatarButton = new QPushButton(this);  // 头像
    this->m_username = new QLabel(this);           // 用户名
    this->m_timestamp = new QLabel(this);          // 时间
    // ChatMessage通过unique_ptr拥有消息副本，避免调用方传入局部变量后产生悬空指针。
    this->m_chatMessage = new ChatMessage(data, isLeft, textFont, this);

    // 初始化UI界面
    this->_InitMessageItem();
}

MessageItem::~MessageItem() = default;

std::unique_ptr<MessageItem> MessageItem::CreateMessageItem(QWidget *parent, const Model::Message &data, bool isLeft,
                                                            const QFont &textFont)
{
    return std::make_unique<MessageItem>(parent, data, isLeft, textFont);
}

void MessageItem::_InitMessageItem()
{
    if (this->m_avatarButton == nullptr || this->m_chatMessage == nullptr || this->m_timestamp == nullptr ||
        this->m_username == nullptr)
    {
        LogInfo(LogLevel::ERROR, "消息项资源初始化失败");
        exit(-1);
    }

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    // 布局管理
    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(24, 10, 24, 10);
    layout->setHorizontalSpacing(12);
    layout->setVerticalSpacing(6);

    // 设置头像按钮样式
    QIcon avatar = this->m_chatMessage->m_message->m_sender.m_avatar;
    if (avatar.isNull()) { avatar = QIcon(":/images/defaultAvatar.png"); }
    this->m_avatarButton->setIcon(avatar);
    this->m_avatarButton->setIconSize(QSize(44, 44));
    this->m_avatarButton->setFixedSize(44, 44);
    this->m_avatarButton->setObjectName("messageAvatarButton");
    this->m_avatarButton->setProperty("variant", "icon");
    this->m_avatarButton->setCursor(Qt::PointingHandCursor);
    this->m_avatarButton->setAccessibleDescription("查看发送者资料");
    connect(this->m_avatarButton, &QPushButton::clicked, this, &MessageItem::_ShowUserInfoWidget);

    // 设置用户名标签样式
    this->m_username->setText(DisplaySenderName(this->m_chatMessage->m_message->m_sender));
    this->m_username->setAlignment(this->m_isLeft ? Qt::AlignLeft | Qt::AlignVCenter
                                                   : Qt::AlignRight | Qt::AlignVCenter);
    this->m_username->setObjectName("usernameLabel");
    this->m_username->setProperty("side", this->m_isLeft ? "left" : "right");
    // 设置时间标签样式
    this->m_timestamp->setText(DisplayMessageTime(this->m_chatMessage->m_message->m_timestamp));
    this->m_timestamp->setAlignment(Qt::AlignCenter);
    this->m_timestamp->setObjectName("timestampLabel");

    auto *messageLayout = new QVBoxLayout();
    messageLayout->setContentsMargins(0, 0, 0, 0);
    messageLayout->setSpacing(5);

    // 根据消息位置(左侧或右侧)设置布局
    if (this->m_isLeft)
    {
        // 对方消息：昵称位于气泡上方，头像在左侧。
        messageLayout->addWidget(this->m_username, 0, Qt::AlignLeft);
        messageLayout->addWidget(this->m_chatMessage, 0, Qt::AlignLeft);
        layout->addWidget(this->m_timestamp, 0, 0, 1, 3, Qt::AlignCenter);
        layout->addWidget(this->m_avatarButton, 1, 0, Qt::AlignTop | Qt::AlignLeft);
        layout->addLayout(messageLayout, 1, 1, Qt::AlignTop | Qt::AlignLeft);
        layout->setColumnStretch(2, 1);
    }
    else
    {
        // 自己消息：昵称与气泡右对齐，头像在右侧。
        messageLayout->addWidget(this->m_username, 0, Qt::AlignRight);
        messageLayout->addWidget(this->m_chatMessage, 0, Qt::AlignRight);
        layout->addWidget(this->m_timestamp, 0, 0, 1, 3, Qt::AlignCenter);
        layout->setColumnStretch(0, 1);
        layout->addLayout(messageLayout, 1, 1, Qt::AlignTop | Qt::AlignRight);
        layout->addWidget(this->m_avatarButton, 1, 2, Qt::AlignTop | Qt::AlignRight);
    }

    // 设置消息内容,根据消息类型分类
    this->setLayout(layout);
}

void MessageItem::_ShowUserInfoWidget()
{
    if (this->m_chatMessage == nullptr || this->m_chatMessage->m_message == nullptr ||
        this->m_avatarButton == nullptr)
    {
        return;
    }

    if (this->m_userInfoWidget == nullptr)
    {
        // 交给 MessageItem 的 Qt 对象树管理，QPointer 只保存观察引用。
        this->m_userInfoWidget = new UserInfoWidget(this->m_chatMessage->m_message->m_sender, this);
    }

    const int horizontalOffset = this->m_isLeft ? this->m_avatarButton->width() + 12
                                                : -this->m_userInfoWidget->width() - 12;
    const QPoint popupPosition = this->m_avatarButton->mapToGlobal(QPoint(horizontalOffset, 0));
    this->m_userInfoWidget->move(popupPosition);
    this->m_userInfoWidget->show();
    this->m_userInfoWidget->raise();
}
