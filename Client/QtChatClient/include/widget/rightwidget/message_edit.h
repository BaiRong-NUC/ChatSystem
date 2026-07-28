#pragma once
#include <public.h>
#include <utils/log.h>

namespace ChatWidget
{
    class MessageEdit : public QWidget
    {
        Q_OBJECT
       private:
        void _InitMessageEdit();  // 初始化消息输入框
       public:
        explicit MessageEdit(QWidget *parent = nullptr);
        ~MessageEdit() override;

        QPointer<QPlainTextEdit> m_textEdit;
        QPointer<QPushButton> m_sendButton;

        QPointer<QPushButton> m_sendImageButton;
        QPointer<QPushButton> m_sendFileButton;
        QPointer<QPushButton> m_sendSpeechButton;
        QPointer<QPushButton> m_historyButton;
    };
}  // namespace ChatWidget
