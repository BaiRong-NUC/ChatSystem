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
        void _UpdateSignatureDisplay();
        bool eventFilter(QObject *watched, QEvent *event) override;

       public:
        explicit SelfWidget(const Model::UserInfo &userInfo, QWidget *parent = nullptr);
        ~SelfWidget() override = default;

        QPointer<QPushButton> m_avatarButton;
        QPointer<QLabel> m_userIdValueLabel;

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
