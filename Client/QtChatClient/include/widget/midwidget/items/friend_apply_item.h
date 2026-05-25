#pragma once
#include <public.h>
#include <utils/log.h>
#include <widget/midwidget/base/base_item.h>
namespace ChatWidget
{
    // 好友申请item
    class FriendApplyItem : public BaseItem
    {
        Q_OBJECT
       private:
        // 申请人的用户id
        QString m_friend_user_id;

       public:
        explicit FriendApplyItem(QWidget *owner, const QString &friendUserId, QWidget *parent = nullptr,
                                 const QIcon &friendIcon = QIcon(), const QString &friendName = QString());
        ~FriendApplyItem() override;
    };
}  // namespace ChatWidget