#include <widget/main_widget/midwidget/items/chat_item.h>

using namespace ChatWidget;
using namespace Log;

ChatItem::ChatItem(QWidget *owner, const QString &chatSessionId, QWidget *parent, const QIcon &friendIcon,
                   const QString &friendName, const QString &text)
    : BaseItem(owner, parent, friendIcon, friendName, text)
{
    this->m_chatSessionId = chatSessionId;  // 设置当前聊天会话id
}

void ChatItem::HandleLeftClick()
{
    // 处理左键点击事件，切换到对应的聊天会话
    LogInfo(LogLevel::INFO, "ChatItem clicked: chatSessionId = " + m_chatSessionId);
    // 这里可以添加切换聊天会话的逻辑，例如通知MidWidget切换到对应的聊天界面
}

void ChatItem::HandleRightClick()
{
    // 处理右键点击事件，例如显示上下文菜单
    LogInfo(LogLevel::INFO, "ChatItem right-clicked: chatSessionId = " + m_chatSessionId);
}

ChatItem::~ChatItem() = default;
