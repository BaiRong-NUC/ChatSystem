#include <widget/rightwidget/message_item.h>

using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr auto KIconButtonStyle =
        "QPushButton { background-color: transparent; border: none; border-radius: 20px; }"
        "QPushButton:hover { background-color: #ecf0f1; border: none; border-radius: 20px; }"
        "QPushButton:pressed { background-color: #bdc3c7; border: none; border-radius: 20px; }";
    constexpr auto kTimestampUserLabelStyle = "QLabel#timestampUserLabel { color: #7f8c8d; font-size: 12px; }";
}  // namespace

MessageItem::MessageItem(QWidget *parent, Model::Message *data, bool isLeft) : QWidget(parent), m_isLeft(isLeft)
{
    // 资源初始化
    this->m_avatarButton = new QPushButton(this);  // 头像
    this->m_timestamp = new QLabel(this);          // 时间
    this->m_chatMessage = new ChatMessage(data, this);  // 消息内容控件,根据消息类型创建不同的消息内容控件

    // 初始化UI界面
    this->_InitMessageItem();
}

MessageItem::~MessageItem()
{
    if (this->m_avatarButton != nullptr) { delete this->m_avatarButton; }
    if (this->m_timestamp != nullptr) { delete this->m_timestamp; }
    if (this->m_chatMessage != nullptr) { delete this->m_chatMessage; }
    this->m_avatarButton = nullptr;
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
    if (this->m_avatarButton == nullptr || this->m_chatMessage == nullptr)
    {
        LogInfo(LogLevel::ERROR, "消息项资源初始化失败");
        exit(-1);
    }

    // 设置消息最小高度
    this->setMinimumHeight(100);

    // 布局管理
    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // 设置头像按钮样式
    this->m_avatarButton->setIcon(this->m_chatMessage->m_message->m_sender.m_avatar);
    this->m_avatarButton->setIconSize(QSize(40, 40));
    this->m_avatarButton->setFixedSize(40, 40);
    this->m_avatarButton->setObjectName("iconButton");
    this->m_avatarButton->setStyleSheet(KIconButtonStyle);

    // 根据消息位置(左侧或右侧)设置布局
    if (this->m_isLeft) { layout->addWidget(this->m_avatarButton, 0, 0, 2, 1, Qt::AlignTop | Qt::AlignLeft); }
    else { layout->addWidget(this->m_avatarButton, 0, 1, 2, 1, Qt::AlignTop | Qt::AlignRight); }

    // 设置名字和时间标签样式
    this->m_timestamp->setText(this->m_chatMessage->m_message->m_sender.m_userName + "|" +
                               this->m_chatMessage->m_message->m_timestamp);
    this->m_timestamp->setAlignment(Qt::AlignBottom);
    this->m_timestamp->setObjectName("timestampUserLabel");
    this->m_timestamp->setStyleSheet(kTimestampUserLabelStyle);

    // 根据消息左右设置布局
    if (this->m_isLeft) { layout->addWidget(this->m_timestamp, 1, 0, 1, 1, Qt::AlignBottom | Qt::AlignLeft); }
    else { layout->addWidget(this->m_timestamp, 1, 1, 1, 1, Qt::AlignBottom | Qt::AlignRight); }

    // 添加消息内容控件到布局中,根据消息类型分类
    if (this->m_isLeft) { layout->addWidget(this->m_chatMessage, 0, 1, 1, 1, Qt::AlignTop | Qt::AlignLeft); }
    else { layout->addWidget(this->m_chatMessage, 0, 0, 1, 1, Qt::AlignTop | Qt::AlignRight); }

    // 设置消息内容,根据消息类型分类
    this->setLayout(layout);
}