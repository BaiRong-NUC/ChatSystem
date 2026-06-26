#include <widget/rightwidget/message_item.h>

using namespace ChatWidget;
using namespace Log;

namespace
{
}  // namespace

MessageItem::MessageItem(QWidget *parent, Model::Message *data, bool isLeft)
    : QWidget(parent), m_data(data), m_isLeft(isLeft)
{
    
}

MessageItem::~MessageItem()
{
    if (this->m_avatarButton != nullptr)
    {
        delete this->m_avatarButton;
        this->m_avatarButton = nullptr;
    }
    if (this->m_data != nullptr)
    {
        delete this->m_data;
        this->m_data = nullptr;
    }
}

MessageItem *MessageItem::CreateMessageItem(QWidget *parent, Model::Message *data, bool isLeft)
{
    MessageItem *item = new MessageItem(parent, data, isLeft);
    // 设置消息最小高度
    item->setMinimumHeight(100);

    // 布局管理
    QGridLayout *layout = new QGridLayout(item);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    item->setLayout(layout);

    return item;
}