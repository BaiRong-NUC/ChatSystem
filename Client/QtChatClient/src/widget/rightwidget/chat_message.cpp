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
    // 初始化UI
}