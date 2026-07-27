#include <widget/rightwidget/chat_message.h>

using namespace ChatWidget;
using namespace Model;
using namespace Log;

namespace
{

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
            break;
        case Model::MessageType::Image:
            break;
        case Model::MessageType::File:
            break;
        case Model::MessageType::SPEECH:
            break;
        default:
            LogInfo(LogLevel::ERROR, "未知的消息类型");
            exit(-1);
    }
}