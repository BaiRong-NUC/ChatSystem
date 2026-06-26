#pragma once
#include <utils/utils.h>
#include <utils/log.h>
#include <model/data.h>
namespace ChatWidget
{
    class MessageItem : public QWidget
    {
        Q_OBJECT
       public:
        explicit MessageItem(QWidget *parent = nullptr, Model::Message *data = nullptr, bool isLeft = true);
        ~MessageItem() override;

        void SetText(const QString &text);
        QString GetText() const;

        // 工厂模式创建对象
        static MessageItem *CreateMessageItem(QWidget *parent, Model::Message *data, bool isLeft);

       public:
        Model::Message *m_data;       // 消息数据
        bool m_isLeft;                // 是否是左侧消息,左侧消息和右侧消息不同
        QPushButton *m_avatarButton;  // 消息发送者头像按钮
        QDateTime m_timestamp;        // 消息发送的时间
    };
}  // namespace ChatWidget