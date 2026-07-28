#pragma once
#include <public.h>
#include <utils/log.h>
#include <widget/main_widget/midwidget/base/base_item.h>
namespace ChatWidget
{
    // 好友申请item
    class FriendApplyItem : public BaseItem
    {
        Q_OBJECT
       private:
        void _InitFriendApplyItem();  // 初始化好友申请项UI
        // 保存两个按钮窗口
        QPointer<QWidget> m_buttonContainerWidget;

       public:
        explicit FriendApplyItem(QWidget *owner, const QString &friendUserId, QWidget *parent = nullptr,
                                 const QIcon &friendIcon = QIcon(), const QString &friendName = QString());
        ~FriendApplyItem() override;

        // 重写点击事件处理函数
        void HandleLeftClick() override;
        void HandleRightClick() override;

        // 申请人的用户id
        QString m_friend_user_id;

        // 同意好友申请按钮
        QPointer<QPushButton> m_acceptButton;

        // 拒绝好友申请按钮
        QPointer<QPushButton> m_rejectButton;
    };
}  // namespace ChatWidget
