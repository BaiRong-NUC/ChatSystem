#include <widget/midwidget/friend_item.h>

using namespace ChatWidget;
using namespace Log;

FriendItem::FriendItem(QWidget *owner, const QString &friendUserId, QWidget *parent, const QIcon &friendIcon,
                       const QString &friendName, const QString &description)
    : BaseItem(owner, parent, friendIcon, friendName, description)
{
    this->m_friend_user_id = friendUserId;  // 设置好友用户id
}

FriendItem::~FriendItem() = default;