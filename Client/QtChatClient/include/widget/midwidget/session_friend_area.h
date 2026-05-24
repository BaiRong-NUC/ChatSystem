#pragma once
#include <public.h>
namespace ChatWidget
{
    class SessionFriendArea : public QScrollArea
    {
        Q_OBJECT
       private:
        void _InitSessionFriendArea();  // 初始化好友信息区域
       public:
        ~SessionFriendArea() override;
        explicit SessionFriendArea(QWidget *parent = nullptr);
        QWidget *m_friendListWidget;  // 好友列表容器
    };
}  // namespace ChatWidget