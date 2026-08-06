#include <widget/main_widget/midwidget/mid_session_area_widget.h>

using namespace ChatWidget;
using namespace Log;

MidSessionAreaWidget::MidSessionAreaWidget(QWidget *parent) : AutoHideScrollArea(parent)
{
    // 初始化资源
    this->m_sessionListWidget = new QWidget(this);  // 创建会话列表容器
    // 初始化窗口
    this->_InitSessionArea();
}

MidSessionAreaWidget::~MidSessionAreaWidget() = default;

void MidSessionAreaWidget::_InitSessionArea()
{
    if (this->m_sessionListWidget == nullptr)
    {
        LogInfo(LogLevel::ERROR, "sessionListWidget资源初始化失败");
        exit(-1);
    }
    // 设置背景
    this->setObjectName("midSessionAreaWidget");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->m_sessionListWidget->setObjectName("sessionListWidget");
    this->m_sessionListWidget->setAttribute(Qt::WA_StyledBackground, true);

    // 自动隐藏滚动条行为由公共AutoHideScrollArea提供。
    this->setWidgetResizable(true);  // 设置滚动区域可调整大小

    this->m_sessionListWidget->setFixedWidth(this->viewport()->width());  // 会话列表宽度跟随视口
    this->setWidget(this->m_sessionListWidget);  // 将会话列表容器设置为滚动区域的子组件

    // 布局
    QVBoxLayout *sessionListLayout = new QVBoxLayout(this->m_sessionListWidget);
    sessionListLayout->setContentsMargins(0, 0, 0, 0);  // 设置布局边距为0
    sessionListLayout->setSpacing(0);
    sessionListLayout->setAlignment(Qt::AlignTop);            // 设置布局对齐方式为顶部对齐
    this->m_sessionListWidget->setLayout(sessionListLayout);  // 将布局设置为会话列表容器的布局

    this->RefreshScrollBar();

#if DEBUG_CODE
    for (int i = 0; i < 20; ++i)
    {
        // this->AddItem(FriendItemType, QString("friend%1").arg(i + 1), QIcon(":/images/defaultAvatar.png"),
        //               QString("好友%1").arg(i + 1), QString("最后一条消息%1").arg(i + 1));

        // this->AddItem(ChatItemType, QString("friend%1").arg(i + 1), QIcon(":/images/defaultAvatar.png"),
        //               QString("好友%1").arg(i + 1), QString("最后一条消息%1").arg(i + 1));

        this->AddItem(ChatItemType, QString("friend%1").arg(i + 1), QIcon(":/images/defaultAvatar.png"),
                      QString("好友%1").arg(i + 1), QString("最后一条消息 %1").arg(i + 1));
    }
#endif
}

void MidSessionAreaWidget::resizeEvent(QResizeEvent *event)
{
    AutoHideScrollArea::resizeEvent(event);

    if (this->m_sessionListWidget != nullptr) { this->m_sessionListWidget->setFixedWidth(this->viewport()->width()); }
}

bool MidSessionAreaWidget::ClearSessionList()
{
    // 获取会话列表布局
    QVBoxLayout *sessionListLayout = qobject_cast<QVBoxLayout *>(this->m_sessionListWidget->layout());
    if (sessionListLayout == nullptr)
    {
        LogInfo(LogLevel::ERROR, "sessionListLayout资源获取失败");
        exit(-1);
    }

    // 删除所有会话项
    while (auto child = std::unique_ptr<QLayoutItem>(sessionListLayout->takeAt(0)))
    {
        // 立即销毁而不是deleteLater，清空操作完成时内存也已经释放。
        std::unique_ptr<QWidget> widget(child->widget());
    }
    this->RefreshScrollBar();
    return true;
}

bool MidSessionAreaWidget::AddItem(const ItemType &type, const QString &id, const QIcon &friendIcon,
                                   const QString &friendName, const QString &lastMessage)
{
    std::unique_ptr<BaseItem> item;
    if (type == ChatItemType)
    {
        item = std::make_unique<ChatItem>(this, id, this->m_sessionListWidget, friendIcon, friendName, lastMessage);
    }
    else if (type == FriendItemType)
    {
        item = std::make_unique<FriendItem>(this, id, this->m_sessionListWidget, friendIcon, friendName, lastMessage);
    }
    else if (type == FriendApplyItemType)
    {
        item = std::make_unique<FriendApplyItem>(this, id, this->m_sessionListWidget, friendIcon, friendName);
    }
    else
    {
        LogInfo(LogLevel::WARNING, "未知的项类型: " + QString::number(static_cast<int>(type)));
        return false;
    }

    if (item == nullptr)
    {
        LogInfo(LogLevel::ERROR, "项创建失败");
        return false;
    }

    // 将项添加到会话列表布局
    QVBoxLayout *sessionListLayout = qobject_cast<QVBoxLayout *>(this->m_sessionListWidget->layout());
    if (sessionListLayout == nullptr)
    {
        LogInfo(LogLevel::ERROR, "sessionListLayout资源获取失败");
        exit(-1);
    }
    sessionListLayout->addWidget(item.release());  // 所有权交给m_sessionListWidget的Qt对象树
    this->RefreshScrollBar();
    return true;
}

// 选中特定的会话项
bool MidSessionAreaWidget::SelectSessionItem(int index)
{
    QVBoxLayout *sessionListLayout = qobject_cast<QVBoxLayout *>(this->m_sessionListWidget->layout());
    if (sessionListLayout == nullptr)
    {
        LogInfo(LogLevel::ERROR, "sessionListLayout资源获取失败");
        exit(-1);
    }

    int itemCount = sessionListLayout->count();
    if (index < 0 || index >= itemCount)
    {
        LogInfo(LogLevel::WARNING, "选中会话项索引越界: " + QString::number(index));
        return false;
    }
    QLayoutItem *layoutItem = sessionListLayout->itemAt(index);
    if (layoutItem == nullptr || layoutItem->widget() == nullptr)
    {
        LogInfo(LogLevel::ERROR, "选中会话项资源获取失败");
        return false;
    }

    BaseItem *item = qobject_cast<BaseItem *>(layoutItem->widget());
    if (item != nullptr)
    {
        // 发送点击事件,触发选中逻辑
        QMouseEvent clickEvent(QEvent::MouseButtonPress, QPointF(1, 1), QPointF(1, 1), QPointF(1, 1), Qt::LeftButton,
                               Qt::LeftButton, Qt::NoModifier);
        QCoreApplication::sendEvent(item, &clickEvent);
    }
    return true;
}
