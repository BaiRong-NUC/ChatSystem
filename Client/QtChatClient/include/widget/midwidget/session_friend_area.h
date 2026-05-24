#pragma once
#include <public.h>
#include <widget/midwidget/session_friend_item.h>
#include <utils/log.h>
namespace ChatWidget
{
    class SessionFriendArea : public QScrollArea
    {
        Q_OBJECT
       private:
        void _InitSessionFriendArea();            // 初始化好友信息区域
        void _SetScrollBarVisible(bool visible);  // 按悬停状态切换滚动条显示

       protected:
        bool eventFilter(QObject *watched, QEvent *event) override;

       public:
        ~SessionFriendArea() override;
        explicit SessionFriendArea(QWidget *parent = nullptr);
        QWidget *m_friendListWidget;  // 好友列表容器

        bool ClearFriendList();  // 清空好友列表

        bool AddFriendItem(const QIcon &friendIcon, const QString &friendName,
                           const QString &lastMessage);  // 添加好友项
    };
}  // namespace ChatWidget