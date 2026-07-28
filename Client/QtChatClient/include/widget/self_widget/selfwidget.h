#pragma once

#include <QDialog>
#include <QString>

class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

namespace Model
{
    class UserInfo;
}

namespace ChatWidget
{
    class SelfWidget final : public QDialog
    {
        Q_OBJECT

        void _InitSelfWidget();  // 初始化UI界面

       public:
        explicit SelfWidget(QWidget *parent = nullptr);
        ~SelfWidget() override = default;

        struct EditableRow
        {
            QLabel *titleLabel = nullptr;
            QLabel *valueLabel = nullptr;
            QLineEdit *editor = nullptr;
            QPushButton *editButton = nullptr;
            QPushButton *submitButton = nullptr;
        };
        QPushButton *m_avatarButton = nullptr;
        QLabel *m_userIdValueLabel = nullptr;

        EditableRow m_userNameRow;
        EditableRow m_userTagRow;
        EditableRow m_signatureRow;
        EditableRow m_phoneRow;

        QLabel *m_phoneVerificationStatusLabel = nullptr;
        QLabel *m_phoneVerificationCodeTitleLabel = nullptr;
        QLineEdit *m_phoneVerificationCodeEdit = nullptr;
        QPushButton *m_submitVerificationCodeButton = nullptr;
        QLabel *m_feedbackLabel = nullptr;
    };
}  // namespace ChatWidget
