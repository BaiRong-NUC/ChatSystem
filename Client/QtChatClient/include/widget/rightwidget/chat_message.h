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

       public:
        explicit ChatMessage(Model::Message *message = nullptr, QWidget *parent = nullptr);
        ~ChatMessage() override;

        // 聊天消息
        Model::Message *m_message = nullptr;
    };
}  // namespace ChatWidget