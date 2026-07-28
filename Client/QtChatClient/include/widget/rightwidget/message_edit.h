#pragma once
#include <public.h>
#include <utils/log.h>

namespace ChatWidget
{
    class MessageEdit : public QWidget
    {
        Q_OBJECT
       private:
        explicit MessageEdit(QWidget *parent = nullptr);
        void _InitMessageEdit();         // 初始化消息输入框
        static MessageEdit *s_instance;  // 单例实例指针
       public:
        ~MessageEdit() override;
        static MessageEdit *GetInstance(QWidget *parent = nullptr);  // 获取单例实例的静态方法

        QPlainTextEdit *m_textEdit = nullptr;
        QPushButton *m_sendButton = nullptr;

        QPushButton *m_sendImageButton = nullptr;
        QPushButton *m_sendFileButton = nullptr;
        QPushButton *m_sendSpeechButton = nullptr;
        QPushButton *m_historyButton = nullptr;
    };
}  // namespace ChatWidget
