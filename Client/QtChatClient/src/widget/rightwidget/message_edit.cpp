#include <widget/rightwidget/message_edit.h>

using namespace ChatWidget;
using namespace Log;

MessageEdit *MessageEdit::s_instance = nullptr;  // 初始化单例实例指针

MessageEdit *MessageEdit::GetInstance(QWidget *parent)
{
    if (s_instance == nullptr) { s_instance = new MessageEdit(parent); }
    return s_instance;
}

MessageEdit::MessageEdit(QWidget *parent)
{
    // 资源初始化

    // 初始化UI
    this->_InitMessageEdit();
}

void MessageEdit::_InitMessageEdit()
{
    this->setPlaceholderText("请输入消息内容...");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // 固定高度
    this->setMinimumHeight(40);
}

MessageEdit::~MessageEdit() {}