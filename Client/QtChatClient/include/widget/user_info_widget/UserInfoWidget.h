#pragma once

#include <public.h>
#include <utils/log.h>
#include <model/data.h>

namespace ChatWidget
{
    class UserInfoWidget : public QWidget
    {
        Q_OBJECT

       private:
        void _InitUserInfoWidget(const Model::UserInfo &userInfo);  // 初始化UI界面

       public:
        explicit UserInfoWidget(QWidget *parent = nullptr);
        ~UserInfoWidget() override = default;

        Message m_message;  // 消息对象,用于显示消息

        QPointer<QPushButton> m_avatarButton;  // 聊天对象的头像
        QPointer<QLabel> m_userNameTag;  // 聊天对象的用户名

    };
}  // namespace ChatWidget
