#include <model/data.h>

#include <QDateTime>

namespace Model
{
    // 生成唯一m_messageId,取QUuid的字符串形式的最后12位作为消息ID,保证唯一性
    QString Message::_GenerateUniqueMessageId()
    {
        QString uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
        // uuid.remove('-');
        return "M" + uuid.right(12);
    }

    QString Message::_GetCurrentTimestamp() { return QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"); }

    Message Message::CreateMessage(MessageType type, const QString &chatSessionId, const UserInfo &sender,
                                   const QByteArray &content, const QString &extraInfo)
    {
        if (type == MessageType::Text) { return _CreateTextMessage(chatSessionId, sender, content); }
        else if (type == MessageType::Image) { return _CreateImageMessage(chatSessionId, sender, content); }
        else if (type == MessageType::File) { return _CreateFileMessage(chatSessionId, sender, content, extraInfo); }
        else if (type == MessageType::SPEECH) { return _CreateSpeechMessage(chatSessionId, sender, content); }
        else
        {
            // 未知消息类型默认创建空消息对象
            return Message();
        }
    }

    Message Message::_CreateTextMessage(const QString &chatSessionId, const UserInfo &sender, const QByteArray &text)
    {
        Message msg;
        msg.m_messageType = MessageType::Text;
        msg.m_chatSessionId = chatSessionId;
        msg.m_sender = sender;
        msg.m_content = text;
        msg.m_attachmentId = "";                                // 文本消息没有附件ID
        msg.m_documentName = "";                                // 文本消息没有文件名
        msg.m_messageId = Message::_GenerateUniqueMessageId();  // 生成唯一消息ID
        msg.m_timestamp = Message::_GetCurrentTimestamp();      // 设置当前时间戳
        return msg;
    }

    Message Message::_CreateImageMessage(const QString &chatSessionId, const UserInfo &sender,
                                         const QByteArray &imageData)
    {
        Message msg;
        msg.m_messageType = MessageType::Image;
        msg.m_chatSessionId = chatSessionId;
        msg.m_sender = sender;
        msg.m_content = imageData;
        msg.m_attachmentId = Message::_GenerateUniqueMessageId();
        msg.m_documentName = "";
        msg.m_messageId = Message::_GenerateUniqueMessageId();
        msg.m_timestamp = Message::_GetCurrentTimestamp();
        return msg;
    }

    Message Message::_CreateFileMessage(const QString &chatSessionId, const UserInfo &sender,
                                        const QByteArray &fileData, const QString &extraInfo)
    {
        Message msg;
        msg.m_messageType = MessageType::File;
        msg.m_chatSessionId = chatSessionId;
        msg.m_sender = sender;
        msg.m_content = fileData;
        msg.m_attachmentId = Message::_GenerateUniqueMessageId();
        msg.m_documentName = extraInfo;
        msg.m_messageId = Message::_GenerateUniqueMessageId();
        msg.m_timestamp = Message::_GetCurrentTimestamp();
        return msg;
    }

    Message Message::_CreateSpeechMessage(const QString &chatSessionId, const UserInfo &sender,
                                          const QByteArray &speechData)
    {
        Message msg;
        msg.m_messageType = MessageType::SPEECH;
        msg.m_chatSessionId = chatSessionId;
        msg.m_sender = sender;
        msg.m_content = speechData;
        msg.m_attachmentId = Message::_GenerateUniqueMessageId();
        msg.m_documentName = "";
        msg.m_messageId = Message::_GenerateUniqueMessageId();
        msg.m_timestamp = Message::_GetCurrentTimestamp();
        return msg;
    }
}  // namespace Model