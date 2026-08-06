#pragma once
#include <utils/utils.h>
#include <utils/log.h>
#include <model/data.h>
#include <widget/main_widget/rightwidget/chat_message.h>
#include <widget/user_info_widget/user_info_widget.h>
namespace ChatWidget
{
    class MessageItem : public QWidget
    {
        Q_OBJECT
       private:
        void _InitMessageItem();  // 初始化消息项UI界面
        void _ShowUserInfoWidget();

       public:
        explicit MessageItem(QWidget *parent, const Model::Message &data, bool isLeft = true,
                             const QFont &textFont = QFont(DEFAULT_CHAT_FONT, 16));
        ~MessageItem() override;

        void SetText(const QString &text);
        QString GetText() const;

        // 工厂模式创建对象
        static std::unique_ptr<MessageItem> CreateMessageItem(QWidget *parent, const Model::Message &data, bool isLeft,
                                                              const QFont &textFont);

       public:
        bool m_isLeft;                              // 是否是左侧消息,左侧消息和右侧消息不同
        QPointer<QPushButton> m_avatarButton;       // 消息发送者头像按钮
        QPointer<QLabel> m_timestamp;               // 消息发送的时间
        QPointer<QLabel> m_username;                // 消息发送者的用户名
        QPointer<ChatMessage> m_chatMessage;        // Qt父对象拥有，成员仅观察
        QPointer<UserInfoWidget> m_userInfoWidget;  // 消息项拥有资料窗，成员仅观察
    };
}  // namespace ChatWidget
