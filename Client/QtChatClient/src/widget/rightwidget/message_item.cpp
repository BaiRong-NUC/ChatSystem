#include <widget/rightwidget/message_item.h>

using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr auto KIconButtonStyle =
        "QPushButton { background-color: transparent; border: none; border-radius: 20px; }"
        "QPushButton:hover { background-color: #ecf0f1; border: none; border-radius: 20px; }"
        "QPushButton:pressed { background-color: #bdc3c7; border: none; border-radius: 20px; }";
    constexpr auto kUsernameLabelStyle = "QLabel#usernameLabel { color: #e4e5e6; font-size: 14px; font-weight: bold; }";
    constexpr auto kTimestampLabelStyle = "QLabel#timestampLabel { color: #7f8c8d; font-size: 12px; }";
}  // namespace

MessageItem::MessageItem(QWidget *parent, Model::Message *data, bool isLeft) : QWidget(parent), m_isLeft(isLeft)
{
    // 资源初始化
    this->m_avatarButton = new QPushButton(this);  // 头像
    this->m_username = new QLabel(this);           // 用户名
    this->m_timestamp = new QLabel(this);          // 时间
    this->m_chatMessage = new ChatMessage(data, this);  // 消息内容控件,根据消息类型创建不同的消息内容控件

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

    // 设置消息最小高度
    this->setMinimumHeight(100);

    // 布局管理
    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(4);

    // 第一行展示用户名和发送时间，第二行展示消息内容
    QHBoxLayout *messageInfoLayout = new QHBoxLayout();
    messageInfoLayout->setContentsMargins(0, 0, 0, 0);
    messageInfoLayout->setSpacing(16);

    // 设置头像按钮样式
    this->m_avatarButton->setIcon(this->m_chatMessage->m_message->m_sender.m_avatar);
    this->m_avatarButton->setIconSize(QSize(40, 40));
    this->m_avatarButton->setFixedSize(40, 40);
    this->m_avatarButton->setObjectName("iconButton");
    this->m_avatarButton->setStyleSheet(KIconButtonStyle);

    // 设置用户名标签样式
    this->m_username->setText(this->m_chatMessage->m_message->m_sender.m_userName);
    this->m_username->setAlignment(Qt::AlignBottom);
    this->m_username->setObjectName("usernameLabel");
    this->m_username->setStyleSheet(kUsernameLabelStyle);
    // 设置时间标签样式
    this->m_timestamp->setText(this->m_chatMessage->m_message->m_timestamp);
    this->m_timestamp->setAlignment(Qt::AlignBottom);
    this->m_timestamp->setObjectName("timestampLabel");
    this->m_timestamp->setStyleSheet(kTimestampLabelStyle);

    // 根据消息位置(左侧或右侧)设置布局
    if (this->m_isLeft)
    {
        // 对方消息：头像在左，右边依次为“名字 日期”和消息内容
        messageInfoLayout->addWidget(this->m_username);
        messageInfoLayout->addWidget(this->m_timestamp);
        messageInfoLayout->addStretch();

        layout->addWidget(this->m_avatarButton, 0, 0, 2, 1, Qt::AlignTop | Qt::AlignLeft);
        layout->addLayout(messageInfoLayout, 0, 1, Qt::AlignLeft | Qt::AlignBottom);
        layout->addWidget(this->m_chatMessage, 1, 1, Qt::AlignTop | Qt::AlignLeft);
        layout->setColumnStretch(2, 1);
    }
    else
    {
        // 自己消息：左边依次为“日期 名字”和消息内容，头像在右
        messageInfoLayout->addStretch();
        messageInfoLayout->addWidget(this->m_timestamp);
        messageInfoLayout->addWidget(this->m_username);

        layout->setColumnStretch(0, 1);
        layout->addLayout(messageInfoLayout, 0, 1, Qt::AlignRight | Qt::AlignBottom);
        layout->addWidget(this->m_chatMessage, 1, 1, Qt::AlignTop | Qt::AlignRight);
        layout->addWidget(this->m_avatarButton, 0, 2, 2, 1, Qt::AlignTop | Qt::AlignRight);
    }

    // 设置消息内容,根据消息类型分类
    this->setLayout(layout);
}
