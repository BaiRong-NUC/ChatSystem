#pragma once
#include "public.h"
namespace Model
{
    /**
     * 用户信息类
     */
    class UserInfo
    {
       public:
        QString m_userId;       // 用户ID
        QString m_userName;     // 用户名
        QString m_description;  // 用户签名
        QString m_phone;        // 用户手机号
        QIcon m_avatar;         // 用户头像
    };

    /**
     * 消息类
     */
    enum class MessageType
    {
        Text,    // 文本消息
        Image,   // 图片消息
        File,    // 文件消息
        SPEECH,  // 语音消息
    };

    class Message
    {
       private:
        Message() = default;  // 私有构造函数,禁止外部直接创建消息对象,只能通过工厂方法CreateMessage创建
        // 根据消息类型创建不同类型的消息对象的私有静态方法
        static Message _CreateTextMessage(const QString &chatSessionId, const UserInfo &sender, const QByteArray &text);
        static Message _CreateImageMessage(const QString &chatSessionId, const UserInfo &sender,
                                           const QByteArray &imageData);
        static Message _CreateFileMessage(const QString &chatSessionId, const UserInfo &sender,
                                          const QByteArray &fileData, const QString &extraInfo);
        static Message _CreateSpeechMessage(const QString &chatSessionId, const UserInfo &sender,
                                            const QByteArray &speechData);

        // 生成唯一m_messageId
       public:
        static QString _GenerateUniqueMessageId();

        // 时间格式化,转化为"yyyy-MM-dd HH:mm:ss"格式的字符串
        static QString _GetCurrentTimestamp();

       public:
        UserInfo m_sender;          // 消息发送者信息
        QString m_messageId;        // 消息ID
        QString m_chatSessionId;    // 所属聊天会话ID 会话-消息是一对多关系,通过聊天会话ID关联
        QString m_timestamp;        // 消息发送时间 2026-05-22 14:52:00
        MessageType m_messageType;  // 消息类型
        QByteArray
            m_content;  // 消息内容,根据消息类型不同,内容格式也不同,文本消息为UTF-8编码的字符串,图片,文件,语音消息为二进制数据
        QString m_attachmentId;  // 附件ID,仅文件,图片,语音消息使用,文本消息该字段为空
        QString m_documentName;  // 文件名,仅文件消息使用,其他消息该字段为空(图片,语音文件不显示文件名)

        // 工厂模式构造函数,根据消息类型创建不同类型的消息对象
        static Message CreateMessage(MessageType type, const QString &chatSessionId, const UserInfo &sender,
                                     const QByteArray &content, const QString &extraInfo);
    };

    /**
     * 客户端聊天会话信息
     */
    class ChatSessionInfo
    {
       public:
        QString m_chatSessionId;    // 聊天会话ID
        QString m_chatSessionName;  // 聊天会话名称,单聊为对方用户名,群聊为群名称
        Message m_lastMessage;      // 最后一条消息,Message类自定义类
        QIcon m_chatSessionAvatar;  // 聊天会话头像,单聊为对方头像,群聊为群头像
        QString m_userId;           // 当前用户ID,单聊为对方用户ID,群聊为完整的用户ID列表
    };
}  // namespace Model
