#pragma once

#include <public.h>
#include <utils/log.h>
#include <model/data.h>

namespace ChatWidget
{
    // 聊天消息中点击他人头像后展示的只读资料窗口。
    class UserInfoWidget final : public QDialog
    {
        Q_OBJECT

       private:
        void _InitUserInfoWidget(const Model::UserInfo &userInfo);  // 初始化UI界面

       public:
        explicit UserInfoWidget(const Model::UserInfo &userInfo, QWidget *parent = nullptr);
        ~UserInfoWidget() override = default;

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
