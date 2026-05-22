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

补充说明：

1. 上述数据类型位于 Model 命名空间中。
2. Message 对象通过静态工厂方法 CreateMessage 创建，不直接对外开放默认构造。

## 二. 工具模块

当前公共工具主要位于以下两个头文件中：

1. include/utils/utils.h
2. include/utils/log.h

### 1) Utils

Utils 命名空间提供了一组轻量级通用工具函数，当前包括：

| 函数名               | 返回类型   | 作用                           |
| -------------------- | ---------- | ------------------------------ |
| GetFormattedTime     | QString    | 将时间戳格式化为指定时间字符串 |
| GetCurrentTimestamp  | int64_t    | 获取当前 Unix 秒级时间戳       |
| QByteArrayToQIcon    | QIcon      | 将二进制数据转换为 QIcon       |
| ReadFileToByteArray  | QByteArray | 读取文件内容为二进制数据       |
| WriteByteArrayToFile | bool       | 将二进制数据写入文件           |
| GetFileNameFromPath  | QString    | 从完整路径中提取文件名         |

### 2) Log

Log 命名空间提供了客户端当前使用的日志能力，核心内容如下：

| 项               | 说明                                        |
| ---------------- | ------------------------------------------- |
| LogLevel         | 日志级别枚举，当前支持 INFO、WARNING、ERROR |
| GLOBAL_LOG_LEVEL | 全局日志级别，低于该级别的日志不会输出      |
| SetLogLevel      | 设置当前日志过滤级别                        |
| LogInfo          | 日志宏，在调用点自动捕获文件名与行号        |

日志模块设计要点：

1. LogInfo 通过宏展开在调用处传入 **LINE**，因此输出的是日志调用位置，而不是日志实现内部行号。
2. 输出时使用文件名而不是完整路径，便于终端阅读。
3. 当前输出格式为：[ERROR main.cpp:16]:这是一个错误日志。

示例：

```cpp
SetLogLevel(Log::LogLevel::WARNING);
LogInfo(Log::LogLevel::INFO, "应用程序启动");
LogInfo(Log::LogLevel::WARNING, "这是一个警告日志");
LogInfo(Log::LogLevel::ERROR, "这是一个错误日志");
```

当日志级别设置为 WARNING 时，实际终端输出示例为：

```text
[WARNING main.cpp:15]:这是一个警告日志
[ERROR main.cpp:16]:这是一个错误日志
```
