#include <widget/main_widget/midwidget/items/friend_item.h>

using namespace ChatWidget;
using namespace Log;

FriendItem::FriendItem(QWidget *owner, const QString &friendUserId, QWidget *parent, const QIcon &friendIcon,
                       const QString &friendName, const QString &description)
    : BaseItem(owner, parent, friendIcon, friendName, description)
{
    this->m_friend_user_id = friendUserId;  // 设置好友用户id
}

FriendItem::~FriendItem() = default;

void FriendItem::HandleLeftClick()
{
    // 处理左键点击事件，例如切换到对应的聊天会话
    LogInfo(LogLevel::INFO, "FriendItem clicked: friendUserId = " + m_friend_user_id);
    // 这里可以添加切换聊天会话的逻辑，例如通知MidWidget切换到对应的聊天界面
}

void FriendItem::HandleRightClick()
{
    // 处理右键点击事件，例如显示上下文菜单
    LogInfo(LogLevel::INFO, "FriendItem right-clicked: friendUserId = " + m_friend_user_id);
}
