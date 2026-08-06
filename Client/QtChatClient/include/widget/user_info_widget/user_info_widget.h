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
        void _UpdateSignatureDisplay();
        void _InitHoverCard();
        void _ScheduleHoverCard(QObject *source);
        void _ScheduleHideHoverCard();
        void _ShowHoverCard();
        bool _SupportsHoverCard(QObject *source) const;
        bool eventFilter(QObject *watched, QEvent *event) override;

        QString m_userId;
        QString m_signature;
        QPointer<QWidget> m_hoverCard;
        QPointer<QLabel> m_hoverCardTitleLabel;
        QPointer<QLabel> m_hoverCardContentLabel;
        QPointer<QTimer> m_hoverShowTimer;
        QPointer<QTimer> m_hoverHideTimer;
        QPointer<QObject> m_pendingHoverSource;
        QPointer<QObject> m_activeHoverSource;

       public:
        explicit UserInfoWidget(const Model::UserInfo &userInfo, QWidget *parent = nullptr);
        ~UserInfoWidget() override = default;

       signals:
        void applyFriendRequested(const QString &userId);
        void sendMessageRequested(const QString &userId);
        void deleteFriendRequested(const QString &userId);

       public:
        QPointer<QPushButton> m_avatarButton;  // 聊天对象的头像
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
