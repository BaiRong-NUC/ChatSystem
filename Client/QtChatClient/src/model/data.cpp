#include <model/data.h>

namespace Model
{
    // 生成唯一m_messageId,取QUuid的字符串形式的最后12位作为消息ID,保证唯一性
    QString Message::_GenerateUniqueMessageId()
    {
        QString uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
        // uuid.remove('-');
        return "M" + uuid.right(12);
    }

    Message Message::CreateMessage(MessageType type, const QString &chatSessionId, const UserInfo &sender,
                                   const QByteArray &content, const QString &extraInfo)
    {
        Message msg;
        msg.m_messageId = Message::_GenerateUniqueMessageId();
        msg.m_chatSessionId = chatSessionId;
        msg.m_sender = sender;
        msg.m_content = content;
        msg.m_timestamp = Utils::GetFormattedTime(Utils::GetCurrentTimestamp());
        msg.m_messageType = type;
        // 根据消息类型设置m_attachmentId;m_documentName等字段
        if (type == MessageType::Text)
        {
            msg.m_attachmentId = "";  // 文本消息没有附件ID
            msg.m_documentName = "";  // 文本消息没有文件名
        }
        else if (type == MessageType::Image)
        {
            msg.m_documentName = "";      // 图片消息没有文件名
            msg.m_attachmentId = "TODO";  // TODO:生成图片附件ID
        }
        else if (type == MessageType::File)
        {
            msg.m_documentName = extraInfo;  // 文件消息使用extraInfo作为文件名
            msg.m_attachmentId = "TODO";     // TODO:生成文件附件ID
        }
        else if (type == MessageType::SPEECH)
        {
            msg.m_documentName = "";      // 语音消息没有文件名
            msg.m_attachmentId = "TODO";  // TODO:生成语音附件ID
        }
        return msg;
    }
}  // namespace Model