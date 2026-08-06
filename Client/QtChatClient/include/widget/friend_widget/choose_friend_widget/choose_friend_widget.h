#pragma once
#include <public.h>
#include <utils/utils.h>
#include <utils/log.h>
#include <model/data.h>
#include <widget/friend_widget/choose_friend_widget/choose_friend_item.h>

namespace ChatWidget
{
    class ChooseFriendWidget : public QWidget
    {
        Q_OBJECT
       public:
        explicit ChooseFriendWidget(QWidget *parent = nullptr);
        ~ChooseFriendWidget() override;

        // 在所有好友列表中添加一个好友,并设置非选中
        void AddFriend(const QIcon &icon, const QString &name, bool isSelected = false);

        // 在已选择的好友列表中添加一个好友
        void AddSelectedFriend(const QIcon &icon, const QString &name);

       private:
        void _InitChooseFriendWidget();
        void _InitSignalSlots();

       private:
        QPointer<QWidget> m_totalFriendListWidget;     // 所有的好友
        QPointer<QWidget> m_selectedFriendListWidget;  // 已选择的好友
    };
}  // namespace ChatWidget