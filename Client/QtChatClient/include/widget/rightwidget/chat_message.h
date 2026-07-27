#pragma once
#include <utils/utils.h>
#include <utils/log.h>
#include <model/data.h>

namespace ChatWidget
{
    // 根据消息类型创建不同的消息内容控件,并添加到布局中
    class ChatMessage : public QWidget
    {
        Q_OBJECT
       private:
        // 初始化UI
        void _InitChatMessage();

        void _MakeTextMessage();    // 创建文本消息内容控件
        void _MakeImageMessage();   // 创建图片消息内容控件
        void _MakeFileMessage();    // 创建文件消息内容控件
        void _MakeSpeechMessage();  // 创建语音消息内容控件

       public:
        explicit ChatMessage(Model::Message *message = nullptr, QWidget *parent = nullptr);
        ~ChatMessage() override;

        // 聊天消息
        Model::Message *m_message = nullptr;
    };

    // 文本消息展示
    class TextChatMessage : public QWidget
    {
        Q_OBJECT
       private:
        void _InitTextChatMessage();  // 初始化UI

       public:
        explicit TextChatMessage(QString &text, bool isLeft, QFont *textFont = new QFont("微软雅黑", 16),
                                 QWidget *parent = nullptr);
        ~TextChatMessage() override;
        QFont *m_textFont = nullptr;    // 文本消息字体
        QLabel *m_textLabel = nullptr;  // 文本消息内容控件
        bool m_isLeft = true;           // 是否是左侧消息,左侧消息和右侧消息不同
    };
}  // namespace ChatWidget