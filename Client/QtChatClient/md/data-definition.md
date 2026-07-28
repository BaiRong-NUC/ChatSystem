# QtChatClient 数据定义

本文档描述 QtChatClient 当前的数据模型、消息构造规则以及基础公共能力。

## 1. 核心数据结构

当前核心数据类型位于 include/model/data.h，包含以下结构：

1. UserInfo
2. ChatSessionInfo
3. MessageType
4. Message

上述类型均位于 Model 命名空间中。

### 1.1 UserInfo

| 字段            | 类型    | 含义                 |
| --------------- | ------- | -------------------- |
| m_userId        | QString | 用户 ID              |
| m_userName      | QString | 用户名               |
| m_userTag       | QString | 用户昵称             |
| m_description   | QString | 用户签名             |
| m_phone         | QString | 用户手机号           |
| m_phoneVerified | bool    | 用户手机号是否已验证 |
| m_avatar        | QIcon   | 用户头像             |

### 1.2 ChatSessionInfo

| 字段                | 类型    | 含义                                                           |
| ------------------- | ------- | -------------------------------------------------------------- |
| m_chatSessionId     | QString | 聊天会话 ID                                                    |
| m_chatSessionName   | QString | 聊天会话名称，单聊为对方用户名，群聊为群名称                   |
| m_lastMessage       | Message | 最后一条消息                                                   |
| m_chatSessionAvatar | QIcon   | 会话头像，单聊为对方头像，群聊为群头像                         |
| m_userId            | QString | 当前会话关联用户 ID，单聊为对方用户 ID，群聊为完整用户 ID 列表 |

### 1.3 MessageType

| 枚举值 | 含义     |
| ------ | -------- |
| Text   | 文本消息 |
| Image  | 图片消息 |
| File   | 文件消息 |
| SPEECH | 语音消息 |

### 1.4 Message

| 字段            | 类型        | 含义                                           |
| --------------- | ----------- | ---------------------------------------------- |
| m_sender        | UserInfo    | 消息发送者信息                                 |
| m_messageId     | QString     | 消息 ID                                        |
| m_chatSessionId | QString     | 所属聊天会话 ID                                |
| m_timestamp     | QString     | 消息发送时间，格式示例为 2026-05-22 14:52:00   |
| m_messageType   | MessageType | 消息类型                                       |
| m_content       | QByteArray  | 文本消息为 UTF-8，图片、文件、语音为二进制数据 |
| m_attachmentId  | QString     | 附件 ID，仅文件、图片、语音消息使用            |
| m_documentName  | QString     | 文件名，仅文件消息使用                         |

## 2. Message 工厂方法规则

Message 对象不直接对外开放默认构造，当前通过静态工厂方法 CreateMessage 创建。

当前 CreateMessage 的行为如下：

1. 自动生成以 M 开头的消息 ID。
2. 当前 ID 生成规则为：取 QUuid 字符串最后 12 位作为后缀。
3. 自动写入所属会话 ID、发送者信息、消息内容、消息类型。
4. 自动写入当前格式化时间字符串。
5. extraInfo 参数当前仅在文件消息中使用，对应 m_documentName。

按消息类型划分，当前字段填充规则如下：

| 消息类型 | m_attachmentId | m_documentName |
| -------- | -------------- | -------------- |
| Text     | 空字符串       | 空字符串       |
| Image    | TODO           | 空字符串       |
| File     | TODO           | extraInfo      |
| SPEECH   | TODO           | 空字符串       |

说明：非文本消息当前写入的附件 ID 仍是占位值 TODO，说明附件上传与真实附件 ID 分配流程尚未接入。

## 3. 公共工具模块

当前公共工具主要位于以下头文件中：

1. include/utils/utils.h
2. include/utils/log.h

### 3.1 Utils

Utils 命名空间提供了一组轻量级通用工具函数：

| 函数名               | 返回类型   | 作用                           |
| -------------------- | ---------- | ------------------------------ |
| GetFormattedTime     | QString    | 将时间戳格式化为指定时间字符串 |
| GetCurrentTimestamp  | int64_t    | 获取当前 Unix 秒级时间戳       |
| QByteArrayToQIcon    | QIcon      | 将二进制数据转换为 QIcon       |
| ReadFileToByteArray  | QByteArray | 读取文件内容为二进制数据       |
| WriteByteArrayToFile | bool       | 将二进制数据写入文件           |
| GetFileNameFromPath  | QString    | 从完整路径中提取文件名         |

### 3.2 Log

Log 命名空间提供当前客户端使用的日志能力：

| 项               | 说明                                        |
| ---------------- | ------------------------------------------- |
| LogLevel         | 日志级别枚举，当前支持 INFO、WARNING、ERROR |
| GLOBAL_LOG_LEVEL | 全局日志级别，低于该级别的日志不会输出      |
| SetLogLevel      | 设置当前日志过滤级别                        |
| LogBuilder       | 日志输出实现函数                            |
| LogInfo          | 日志宏，在调用点自动捕获文件名与行号        |

日志模块当前的设计要点：

1. LogInfo 通过宏传入 **LINE**，输出的是实际调用位置。
2. 日志输出使用文件名而不是完整路径，便于终端阅读。
3. 输出格式为 [INFO main.cpp:10]:message 这一类结构。

示例：

```cpp
SetLogLevel(Log::LogLevel::WARNING);
LogInfo(Log::LogLevel::INFO, "应用程序启动");
LogInfo(Log::LogLevel::WARNING, "这是一个警告日志");
LogInfo(Log::LogLevel::ERROR, "这是一个错误日志");
```

当日志级别为 WARNING 时，实际会过滤 INFO，只输出 WARNING 与 ERROR。
