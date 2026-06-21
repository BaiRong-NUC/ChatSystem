#include <widget/rightwidget/message_widget.h>

using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr auto kMessageWidgetStyle =
        "QScrollArea#messageWidget { background-color: #ffffff; border: none; }"
        "QWidget#messageContainer { background-color: #bdc3c7; }";
}

MessageWidget *MessageWidget::s_instance = nullptr;  // 初始化单例实例指针

MessageWidget::MessageWidget(QWidget *parent) : QScrollArea(parent)
{
    // 包含的内容
    this->m_container = new QWidget(this);

    this->_InitMessageWidget();
}

MessageWidget *MessageWidget::GetInstance(QWidget *parent)
{
    if (s_instance == nullptr) { s_instance = new MessageWidget(parent); }
    return s_instance;
}

void MessageWidget::_InitMessageWidget()
{
    // 设置属性
    this->setObjectName("messageWidget");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setWidgetResizable(true);
    // 滚动条
    this->m_container->setObjectName("messageContainer");
    this->m_container->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet(kMessageWidgetStyle);

    QVBoxLayout *messageLayout = new QVBoxLayout(this->m_container);
    messageLayout->setContentsMargins(12, 12, 12, 12);
    messageLayout->setSpacing(12);
    messageLayout->setAlignment(Qt::AlignTop);

    this->setWidget(this->m_container);
}

MessageWidget::~MessageWidget() = default;
