#pragma once
#include <public.h>
#include <utils/log.h>
#include <widget/rightwidget/message_item.h>

namespace ChatWidget
{
    class MessageWidget : public QScrollArea
    {
        Q_OBJECT
       private:
        void _InitMessageWidget();  // 初始化消息信息区
       public:
        explicit MessageWidget(QWidget *parent = nullptr);
        ~MessageWidget() override;

        QPointer<QWidget> m_container;

        // 插入聊天消息
        void AddMessage(bool isLeft, const Model::Message &message,
                        const QFont &textFont = QFont(DEFAULT_CHAT_FONT, 16));  // 尾插
        void AddFrontMessage(bool isLeft, const Model::Message &message,
                             const QFont &textFont = QFont(DEFAULT_CHAT_FONT, 16));  // 头插
        void ClearMessages();  // 清空所有消息
    };
}  // namespace ChatWidget
