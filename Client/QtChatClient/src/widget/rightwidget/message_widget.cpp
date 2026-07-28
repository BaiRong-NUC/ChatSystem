#include <widget/rightwidget/message_widget.h>

using namespace ChatWidget;
using namespace Log;
using namespace Model;

namespace
{
    constexpr auto kMessageWidgetStyle =
        "QScrollArea#messageWidget { background-color: #181818; border: none; }"
        "QWidget#messageContainer { background-color: #181818; }";

    constexpr auto kMessageScollBarStyle =
        "QScrollBar:vertical { background-color: #181818; width: 8px; margin: 0px; }"
        "QScrollBar::handle:vertical { background-color: #4a4a4a; border-radius: 4px; min-height: 24px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background-color: #181818; }";
}  // namespace

MessageWidget *MessageWidget::s_instance = nullptr;  // 初始化单例实例指针

MessageWidget::MessageWidget(QWidget *parent) : QScrollArea(parent)
{
    // 包含的内容
    this->m_container = new QWidget(this);

    this->_InitMessageWidget();
}

MessageWidget::~MessageWidget()
{
    if (this->m_container != nullptr) { delete this->m_container; }
    this->m_container = nullptr;
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

    messageLayout->setContentsMargins(0, 12, 0, 12);
    messageLayout->setSpacing(4);
    messageLayout->setAlignment(Qt::AlignTop);

    // DEBUG
#if DEBUG_CODE
    UserInfo testUser;
    testUser.m_userId = "testUserId";
    testUser.m_userName = "Test User";
    testUser.m_avatar = QIcon(":/resources/images/defaultAvatar.png");
    Model::Message testMessage =
        Model::Message::CreateMessage(Model::MessageType::Text, "chatSessionId", testUser, "Hello, this is a test", "");
    this->AddMessage(true, testMessage);
#endif

    this->setWidget(this->m_container);
}

void MessageWidget::AddMessage(bool isLeft, Model::Message &message, QFont *textFont)
{
    MessageItem *item = MessageItem::CreateMessageItem(this->m_container, &message, isLeft);
    this->m_container->layout()->addWidget(item);
}

void MessageWidget::AddFrontMessage(bool isLeft, Model::Message &message, QFont *textFont)
{
    MessageItem *item = MessageItem::CreateMessageItem(this->m_container, &message, isLeft);
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(this->m_container->layout());
    if (layout != nullptr) { layout->insertWidget(0, item); }
}

void MessageWidget::ClearMessages()
{
    QLayout *layout = this->m_container->layout();
    if (layout != nullptr)
    {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr)
        {
            if (item->widget() != nullptr) { delete item->widget(); }
            delete item;
        }
    }
}
