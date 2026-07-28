#include <widget/rightwidget/message_item.h>

using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr auto KIconButtonStyle =
        "QPushButton { background-color: transparent; border: none; border-radius: 6px; padding: 0px; }"
        "QPushButton:hover { background-color: #2a2a2a; }"
        "QPushButton:pressed { background-color: #333333; }";
    constexpr auto kUsernameLabelStyle = "QLabel#usernameLabel { color: #e4e5e6; font-size: 14px; font-weight: bold; }";
    constexpr auto kTimestampLabelStyle = "QLabel#timestampLabel { color: #8b8b8b; font-size: 12px; }";
}  // namespace

MessageItem::MessageItem(QWidget *parent, Model::Message *data, bool isLeft) : QWidget(parent), m_isLeft(isLeft)
{
    // 资源初始化
    this->m_avatarButton = new QPushButton(this);  // 头像
    this->m_username = new QLabel(this);           // 用户名
    this->m_timestamp = new QLabel(this);          // 时间
    // ChatMessage拥有消息副本，避免调用方传入局部变量后产生悬空指针。
    Model::Message *messageCopy = data != nullptr ? new Model::Message(*data) : nullptr;
    this->m_chatMessage = new ChatMessage(messageCopy, isLeft, this);

    // 初始化UI界面
    this->_InitMessageItem();
}

MessageItem::~MessageItem()
{
    if (this->m_avatarButton != nullptr) { delete this->m_avatarButton; }
    if (this->m_username != nullptr) { delete this->m_username; }
    if (this->m_timestamp != nullptr) { delete this->m_timestamp; }
    if (this->m_chatMessage != nullptr) { delete this->m_chatMessage; }
    this->m_avatarButton = nullptr;
    this->m_username = nullptr;
    this->m_timestamp = nullptr;
    this->m_chatMessage = nullptr;
}

MessageItem *MessageItem::CreateMessageItem(QWidget *parent, Model::Message *data, bool isLeft)
{
    MessageItem *item = new MessageItem(parent, data, isLeft);

    return item;
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
    layout->setVerticalSpacing(8);

    // 设置头像按钮样式
    QIcon avatar = this->m_chatMessage->m_message->m_sender.m_avatar;
    if (avatar.isNull()) { avatar = QIcon(":/images/defaultAvatar.png"); }
    this->m_avatarButton->setIcon(avatar);
    this->m_avatarButton->setIconSize(QSize(44, 44));
    this->m_avatarButton->setFixedSize(44, 44);
    this->m_avatarButton->setObjectName("iconButton");
    this->m_avatarButton->setStyleSheet(KIconButtonStyle);

    // 设置用户名标签样式
    this->m_username->setText(this->m_chatMessage->m_message->m_sender.m_userName);
    this->m_username->setAlignment(Qt::AlignBottom);
    this->m_username->setObjectName("usernameLabel");
    this->m_username->setStyleSheet(kUsernameLabelStyle);
    this->m_username->hide();
    // 设置时间标签样式
    const QDateTime timestamp =
        QDateTime::fromString(this->m_chatMessage->m_message->m_timestamp, "yyyy-MM-dd HH:mm:ss");
    this->m_timestamp->setText(timestamp.isValid() ? timestamp.toString("M月d日 HH:mm")
                                                   : this->m_chatMessage->m_message->m_timestamp);
    this->m_timestamp->setAlignment(Qt::AlignCenter);
    this->m_timestamp->setObjectName("timestampLabel");
    this->m_timestamp->setStyleSheet(kTimestampLabelStyle);

    // 根据消息位置(左侧或右侧)设置布局
    if (this->m_isLeft)
    {
        // 对方消息：头像在左，气泡紧随头像。
        layout->addWidget(this->m_timestamp, 0, 0, 1, 3, Qt::AlignCenter);
        layout->addWidget(this->m_avatarButton, 1, 0, Qt::AlignTop | Qt::AlignLeft);
        layout->addWidget(this->m_chatMessage, 1, 1, Qt::AlignTop | Qt::AlignLeft);
        layout->setColumnStretch(2, 1);
    }
    else
    {
        // 自己消息：气泡靠右，头像在最右侧。
        layout->addWidget(this->m_timestamp, 0, 0, 1, 3, Qt::AlignCenter);
        layout->setColumnStretch(0, 1);
        layout->addWidget(this->m_chatMessage, 1, 1, Qt::AlignTop | Qt::AlignRight);
        layout->addWidget(this->m_avatarButton, 1, 2, Qt::AlignTop | Qt::AlignRight);
    }

    // 设置消息内容,根据消息类型分类
    this->setLayout(layout);
}
