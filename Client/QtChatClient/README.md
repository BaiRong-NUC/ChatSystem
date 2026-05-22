# QtChatClient: QT聊天客户端窗口

## 一. 数据结构:

当前核心数据类型位于 include/model/data.h，包含以下结构：

1. UserInfo（用户信息）
2. ChatSessionInfo（聊天会话信息）
3. MessageType（消息类型枚举）
4. Message（消息实体）

### 1) UserInfo

| 字段          | 类型    | 含义       |
| ------------- | ------- | ---------- |
| m_userId      | QString | 用户ID     |
| m_userName    | QString | 用户名     |
| m_description | QString | 用户签名   |
| m_phone       | QString | 用户手机号 |
| m_avatar      | QIcon   | 用户头像   |

### 2) ChatSessionInfo

| 字段                | 类型    | 含义                                                   |
| ------------------- | ------- | ------------------------------------------------------ |
| m_chatSessionId     | QString | 聊天会话ID                                             |
| m_chatSessionName   | QString | 聊天会话名称，单聊为对方用户名，群聊为群名称           |
| m_lastMessage       | Message | 最后一条消息（Message 类型）                           |
| m_chatSessionAvatar | QIcon   | 会话头像，单聊为对方头像，群聊为群头像                 |
| m_userId            | QString | 当前会话关联用户ID；单聊为对方用户ID，群聊为用户ID列表 |

### 3) MessageType

| 枚举值 | 类型        | 含义     |
| ------ | ----------- | -------- |
| Text   | MessageType | 文本消息 |
| Image  | MessageType | 图片消息 |
| File   | MessageType | 文件消息 |
| SPEECH | MessageType | 语音消息 |

### 4) Message

| 字段            | 类型        | 含义                                               |
| --------------- | ----------- | -------------------------------------------------- |
| m_sender        | UserInfo    | 消息发送者信息（UserInfo）                         |
| m_messageId     | QString     | 消息ID                                             |
| m_chatSessionId | QString     | 所属聊天会话ID（会话与消息为一对多关系）           |
| m_timestamp     | QString     | 消息发送时间，示例：2026-05-22 14:52:00            |
| m_messageType   | MessageType | 消息类型（MessageType）                            |
| m_content       | QByteArray  | 消息内容：文本为 UTF-8；图片/文件/语音为二进制数据 |
| m_attachmentId  | QString     | 附件ID，仅文件/图片/语音消息使用，文本消息为空     |
| m_documentName  | QString     | 文件名，仅文件消息使用，图片和语音消息为空         |
