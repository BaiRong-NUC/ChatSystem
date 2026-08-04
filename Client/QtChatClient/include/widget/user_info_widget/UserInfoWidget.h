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
        QPointer<QLabel> m_userNameTag;        // 名字
        QPointer<QLabel> m_userName;           // 用户名字
        QPointer<QLabel> m_userTag;            // 昵称
        QPointer<QLabel> m_userTagName;        // 用户昵称
        QPointer<QLabel> m_userDescription;    // 用户签名
        QPointer<QLabel> m_userPhone;          // 用户手机号

        QPointer<QPushButton> m_applyfriendButton;  // 申请好友按钮
        QPointer<QPushButton> m_sendMessageButton;  // 发送消息按钮
        QPointer<QPushButton> m_delFriendButton;    // 删除好友按钮
    };
}  // namespace ChatWidget
