#include <widget/rightwidget/chat_message.h>

using namespace ChatWidget;
using namespace Model;
using namespace Log;

namespace
{
    constexpr auto kTextLabelStyle = "QLabel { color: #2c3e50; line-height: 1.5; }";
}

ChatMessage::ChatMessage(Model::Message *message, QWidget *parent) : QWidget(parent), m_message(message)
{
    // 资源初始化
    this->m_message = message;
    // 初始化UI
    this->_InitChatMessage();
}
ChatMessage::~ChatMessage()
{
    if (this->m_message != nullptr) { delete this->m_message; }
    this->m_message = nullptr;
}

void ChatMessage::_InitChatMessage()
{
    if (this->m_message == nullptr)
    {
        LogInfo(LogLevel::ERROR, "消息内容控件资源初始化失败");
        exit(-1);
    }
    // 初始化UI,根据不同的消息类型做处理
    switch (this->m_message->m_messageType)
    {
        case Model::MessageType::Text:
            this->_MakeTextMessage();
            break;
        case Model::MessageType::Image:
            this->_MakeImageMessage();
            break;
        case Model::MessageType::File:
            this->_MakeFileMessage();
            break;
        case Model::MessageType::SPEECH:
            this->_MakeSpeechMessage();
            break;
        default:
            LogInfo(LogLevel::ERROR, "未知的消息类型");
            exit(-1);
    }
}

/**
 *
 *
 *              一. 初始化文本类型消息
 *
 *
 */
// 重写绘制事件,绘制不规则对话框背景
void TextChatMessage::paintEvent(QPaintEvent *event) {}

TextChatMessage::TextChatMessage(QString &text, bool isLeft, QFont *textFont = new QFont("微软雅黑", 16),
                                 QWidget *parent)
    : QWidget(parent), m_isLeft(isLeft), m_textFont(textFont)
{
    // 资源初始化
    this->m_textLabel = new QLabel(this);
    this->m_textLabel->setText(text);
    this->m_textLabel->setFont(*this->m_textFont);
    this->m_textLabel->setWordWrap(true);  // 设置文本自动换行
    this->m_textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    this->m_textLabel->setStyleSheet(kTextLabelStyle);

    // 初始化不规则对话框UI
    this->_InitTextChatMessage();
}

// 初始化不规则对话框UI,适配文字长度
void TextChatMessage::_InitTextChatMessage() {}

void ChatMessage::_MakeTextMessage() {}