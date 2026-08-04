#pragma once

#include <public.h>
#include <utils/log.h>
#include <model/data.h>

namespace ChatWidget
{
    class SelfWidget final : public QDialog
    {
        Q_OBJECT

       private:
        struct EditableRow
        {
            QPointer<QLabel> titleLabel;
            QPointer<QLabel> valueLabel;
            QPointer<QLineEdit> editor;
        };

        void _InitSelfWidget(const Model::UserInfo &userInfo);  // 初始化UI界面
        void _BeginEdit(EditableRow &editableRow);
        void _FinishEdit(EditableRow &editableRow);
        void _UpdatePhoneVerificationUi(bool verified, const QString &description = {});
        void _SubmitPhoneVerificationCode();
        void _UpdateSignatureDisplay();
        void _InitHoverCard();
        void _ScheduleHoverCard(QObject *source);
        void _ScheduleHideHoverCard();
        void _ShowHoverCard();
        bool _SupportsHoverCard(QObject *source) const;
        bool eventFilter(QObject *watched, QEvent *event) override;

        QString m_userId;
        QPointer<QWidget> m_hoverCard;
        QPointer<QLabel> m_hoverCardTitleLabel;
        QPointer<QLabel> m_hoverCardContentLabel;
        QPointer<QTimer> m_hoverShowTimer;
        QPointer<QTimer> m_hoverHideTimer;
        QPointer<QObject> m_pendingHoverSource;
        QPointer<QObject> m_activeHoverSource;

       public:
        explicit SelfWidget(const Model::UserInfo &userInfo, QWidget *parent = nullptr);
        ~SelfWidget() override = default;

        // 服务端确认验证码通过后调用，界面会隐藏验证码输入区域。
        void setPhoneVerified(bool verified);

       signals:
        // 仅负责把用户输入交给业务层，验证结果通过 setPhoneVerified 回写。
        void phoneVerificationRequested(const QString &phone, const QString &verificationCode);

       public:
        QPointer<QPushButton> m_avatarButton;

        EditableRow m_userNameRow;
        EditableRow m_userTagRow;
        EditableRow m_signatureRow;
        EditableRow m_phoneRow;

        QPointer<QWidget> m_phoneDisplayWidget;
        QPointer<QLabel> m_phoneVerificationStatusLabel;
        QPointer<QLabel> m_phoneVerificationCodeTitleLabel;
        QPointer<QLineEdit> m_phoneVerificationCodeEdit;
        QPointer<QPushButton> m_submitVerificationCodeButton;
        QPointer<QLabel> m_feedbackLabel;
    };
}  // namespace ChatWidget
