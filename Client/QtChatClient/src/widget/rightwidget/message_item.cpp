#include <widget/rightwidget/message_item.h>

using namespace ChatWidget;
using namespace Log;

namespace
{
}  // namespace

MessageItem::MessageItem(QWidget *parent, Model::Message *data, bool isLeft)
    : QWidget(parent), m_data(data), m_isLeft(isLeft)
{
    // 资源初始化
    this->m_avatarButton = new QPushButton(this);                                         // 头像
    this->m_timestamp = QDateTime::fromString(data->m_timestamp, "yyyy-MM-dd HH:mm:ss");  // 时间

    // 初始化UI界面
    this->_InitMessageItem();
}

MessageItem::~MessageItem()
{
    if (this->m_avatarButton != nullptr) { delete this->m_avatarButton; }
    if (this->m_data != nullptr) { delete this->m_data; }
    this->m_avatarButton = nullptr;
    this->m_data = nullptr;
}

MessageItem *MessageItem::CreateMessageItem(QWidget *parent, Model::Message *data, bool isLeft)
{
    MessageItem *item = new MessageItem(parent, data, isLeft);

    return item;
}

void MessageItem::_InitMessageItem()
{
    if (this->m_avatarButton == nullptr || this->m_data == nullptr)
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
    this->m_avatarButton->setIcon(this->m_data->m_sender.m_avatar);
    this->m_avatarButton->setIconSize(QSize(40, 40));
    this->m_avatarButton->setFixedSize(40, 40);
    this->m_avatarButton->setStyleSheet(
        "QPushButton { background-color: transparent; border: none; border-radius: 20px; }"
        "QPushButton:hover { background-color: #ecf0f1; border: none; border-radius: 20px; }"
        "QPushButton:pressed { background-color: #bdc3c7; border: none; border-radius: 20px; }");

    // 根据消息位置(左侧或右侧)设置布局
    if (this->m_isLeft) { layout->addWidget(this->m_avatarButton, 0, 0, 2, 1, Qt::AlignTop | Qt::AlignLeft); }
    else { layout->addWidget(this->m_avatarButton, 0, 1, 2, 1, Qt::AlignTop | Qt::AlignRight); }

    
    this->setLayout(layout);
}