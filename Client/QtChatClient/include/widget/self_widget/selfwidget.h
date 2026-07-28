#pragma once

#include <public.h>

namespace ChatWidget
{
    class SelfWidget final : public QDialog
    {
        Q_OBJECT

       public:
        struct EditableRow
        {
            QPointer<QLabel> titleLabel;
            QPointer<QLabel> valueLabel;
            QPointer<QLineEdit> editor;
            QPointer<QPushButton> editButton;
            QPointer<QPushButton> submitButton;
        };

        explicit SelfWidget(QWidget *parent = nullptr);
        ~SelfWidget() override = default;

        QPointer<QPushButton> m_avatarButton;
        QPointer<QLabel> m_userIdValueLabel;

        EditableRow m_userNameRow;
        EditableRow m_userTagRow;
        EditableRow m_signatureRow;
        EditableRow m_phoneRow;

        QPointer<QLabel> m_phoneVerificationStatusLabel;
        QPointer<QLabel> m_phoneVerificationCodeTitleLabel;
        QPointer<QLineEdit> m_phoneVerificationCodeEdit;
        QPointer<QPushButton> m_submitVerificationCodeButton;
        QPointer<QLabel> m_feedbackLabel;

       private:
        void _InitSelfWidget();  // 初始化UI界面
        void _InitEditableRow(EditableRow &editableRow, const QString &title, const QString &placeholder,
                              QGridLayout *mainLayout, int row);
    };
}  // namespace ChatWidget
