#include <widget/midwidget/chat_item.h>

using namespace ChatWidget;
using namespace Log;

ChatItem::ChatItem(QWidget *owner, const QString &chatSessionId, QWidget *parent, const QIcon &friendIcon,
                   const QString &friendName, const QString &text)
    : BaseItem(owner, parent, friendIcon, friendName, text)
{
    this->m_chatSessionId = chatSessionId;  // 设置当前聊天会话id
}

ChatItem::~ChatItem() = default;