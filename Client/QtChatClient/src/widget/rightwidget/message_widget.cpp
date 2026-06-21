#include <widget/rightwidget/message_widget.h>

using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr auto kMessageWidgetStyle =
        "QScrollArea#messageWidget { background-color: #f7f9fa; border: none; }"
        "QWidget#messageContainer { background-color: #f7f9fa; }";
}

MessageWidget *MessageWidget::s_instance = nullptr;  // 初始化单例实例指针

MessageWidget::MessageWidget(QWidget *parent) : QScrollArea(parent) { this->_InitMessageWidget(); }

MessageWidget *MessageWidget::GetInstance(QWidget *parent)
{
    if (s_instance == nullptr) { s_instance = new MessageWidget(parent); }
    return s_instance;
}

void MessageWidget::_InitMessageWidget()
{
    this->setObjectName("messageWidget");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setWidgetResizable(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet(kMessageWidgetStyle);

    QWidget *messageContainer = new QWidget(this);
    messageContainer->setObjectName("messageContainer");
    messageContainer->setAttribute(Qt::WA_StyledBackground, true);

    QVBoxLayout *messageLayout = new QVBoxLayout(messageContainer);
    messageLayout->setContentsMargins(12, 12, 12, 12);
    messageLayout->setSpacing(12);
    messageLayout->setAlignment(Qt::AlignTop);

    this->setWidget(messageContainer);
}

MessageWidget::~MessageWidget() = default;
