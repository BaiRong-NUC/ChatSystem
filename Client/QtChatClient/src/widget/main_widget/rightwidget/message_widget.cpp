#include <widget/main_widget/rightwidget/message_widget.h>

using namespace ChatWidget;
using namespace Log;
using namespace Model;

MessageWidget::MessageWidget(QWidget *parent) : QScrollArea(parent)
{
    // 包含的内容
    this->m_container = new QWidget(this);

    this->_InitMessageWidget();
}

MessageWidget::~MessageWidget() = default;

void MessageWidget::_InitMessageWidget()
{
    // 设置属性
    this->setObjectName("messageWidget");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setWidgetResizable(true);
    // 滚动条区域
    this->m_container->setObjectName("messageContainer");
    this->m_container->setAttribute(Qt::WA_StyledBackground, true);

    // 布局管理器
    QVBoxLayout *messageLayout = new QVBoxLayout(this->m_container);

    messageLayout->setContentsMargins(0, 12, 0, 12);
    messageLayout->setSpacing(4);
    messageLayout->setAlignment(Qt::AlignTop);

    // DEBUG
#if DEBUG_CODE
    UserInfo testUser;
    testUser.m_userId = "testUserId";
    testUser.m_userName = "张三";
    testUser.m_userTag = "小张";
    testUser.m_description = "这是一个测试用户的签名信息，用于展示消息气泡的样式和布局效果。";
    testUser.m_phone = "1234567890";
    testUser.m_avatar = QIcon(":/images/defaultAvatar.png");
    Model::Message testMessage = Model::Message::CreateMessage(Model::MessageType::Text, "chatSessionId", testUser,
                                                               "你好，这是一条测试消息", "");
    for (int i = 0; i < 10; i++) { this->AddMessage(true, testMessage); }

    UserInfo currentUser = testUser;
    currentUser.m_userName = "我";
    Model::Message replyMessage =
        Model::Message::CreateMessage(Model::MessageType::Text, "chatSessionId", currentUser,
                                      QString("测试数据，测试数据，测试数据，测试数据，测试数据，测试数据，测试数据，测"
                                              "试数据，消息气泡也会根据内容自动换行。")
                                          .toUtf8(),
                                      "");
    for (int i = 0; i < 10; i++) { this->AddMessage(false, replyMessage); }
#endif

    this->setWidget(this->m_container);
}

void MessageWidget::AddMessage(bool isLeft, const Model::Message &message, const QFont &textFont)
{
    auto item = MessageItem::CreateMessageItem(this->m_container, message, isLeft, textFont);
    QLayout *layout = this->m_container->layout();
    if (layout->count() > 0)
    {
        MessageItem *previousItem = qobject_cast<MessageItem *>(layout->itemAt(layout->count() - 1)->widget());
        if (previousItem != nullptr && previousItem->m_timestamp->text() == item->m_timestamp->text())
        {
            item->m_timestamp->hide();
        }
    }
    layout->addWidget(item.release());  // 所有权交给m_container的Qt对象树
}

void MessageWidget::AddFrontMessage(bool isLeft, const Model::Message &message, const QFont &textFont)
{
    auto item = MessageItem::CreateMessageItem(this->m_container, message, isLeft, textFont);
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(this->m_container->layout());
    if (layout != nullptr) { layout->insertWidget(0, item.release()); }
}

void MessageWidget::ClearMessages()
{
    QLayout *layout = this->m_container->layout();
    if (layout != nullptr)
    {
        while (auto item = std::unique_ptr<QLayoutItem>(layout->takeAt(0)))
        {
            std::unique_ptr<QWidget> widget(item->widget());
        }
    }
}
