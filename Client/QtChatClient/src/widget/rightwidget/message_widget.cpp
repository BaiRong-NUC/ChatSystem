#include <widget/rightwidget/message_widget.h>

using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr auto kMessageWidgetStyle =
        "QScrollArea#messageWidget { background-color: #ffffff; border: none; }"
        "QWidget#messageContainer { background-color: #bdc3c7; }";

    constexpr auto kMessageScollBarStyle =
        "QScrollBar:vertical { background-color: #bdc3c7; width: 12px; margin: 0px 0px 0px 0px; }"
        "QScrollBar::handle:vertical { background-color: #95a5a6; border-radius: 6px; min-height: 20px; }"
        "QScrollBar::add-line:vertical { background-color: #bdc3c7; height: 12px; subcontrol-position: bottom; "
        "subcontrol-origin: margin; }"
        "QScrollBar::sub-line:vertical { background-color: #bdc3c7; height: 12px; subcontrol-position: top; "
        "subcontrol-origin: margin; }"
        "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical { background-color: #bdc3c7; width: 12px; "
        "height: 12px; }"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background-color: #bdc3c7; }";
}  // namespace

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
    this->setStyleSheet(kMessageWidgetStyle);
    this->setWidgetResizable(true);
    // 滚动条区域
    this->m_container->setObjectName("messageContainer");
    this->m_container->setAttribute(Qt::WA_StyledBackground, true);

    // 隐藏水平滚动条，设置垂直滚动条样式
    this->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 0px; }");
    this->verticalScrollBar()->setStyleSheet(kMessageScollBarStyle);

    // 布局管理器
    QVBoxLayout *messageLayout = new QVBoxLayout(this->m_container);

    messageLayout->setContentsMargins(0, 0, 0, 0);
    messageLayout->setSpacing(0);
    messageLayout->setAlignment(Qt::AlignTop);

    this->setWidget(this->m_container);
}

MessageWidget::~MessageWidget() = default;
