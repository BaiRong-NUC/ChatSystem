#pragma once
#include <public.h>
#include <utils/log.h>
#include <widget/main_widget/midwidget/base/base_item.h>
namespace ChatWidget
{
    // 好友会话item
    class FriendItem : public BaseItem
    {
        Q_OBJECT
       private:
        // 好友用户id
        QString m_friend_user_id;

       public:
        explicit FriendItem(QWidget *owner, const QString &friendUserId, QWidget *parent = nullptr,
                            const QIcon &friendIcon = QIcon(), const QString &friendName = QString(),
                            const QString &description = QString());

        // 重写点击事件处理函数
        void HandleLeftClick() override;
        void HandleRightClick() override;

        ~FriendItem() override;
    };
}  // namespace ChatWidget
