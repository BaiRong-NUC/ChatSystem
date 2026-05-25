#include <widget/midwidget/items/friend_apply_item.h>

using namespace ChatWidget;
using namespace Log;

FriendApplyItem::FriendApplyItem(QWidget *owner, const QString &friendUserId, QWidget *parent, const QIcon &friendIcon,
                                 const QString &friendName)
    : BaseItem(owner, parent, friendIcon, friendName)
{
    this->m_friend_user_id = friendUserId;  // 设置申请人的用户id
}

FriendApplyItem::~FriendApplyItem() = default;
