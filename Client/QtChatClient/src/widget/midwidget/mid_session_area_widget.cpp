#include <widget/midwidget/mid_session_area_widget.h>

using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr int kOverlayScrollBarWidth = 12;

    // constexpr auto 让它们在编译期成为常量字符串
    constexpr auto kMidSessionAreaWidgetStyle =
        "QScrollArea#midSessionAreaWidget { background-color: #949ea9; border: none; }"
        "QWidget#friendListWidget { background-color: #949ea9; }";

    constexpr auto kMidSessionAreaWidgetOverlayScrollBarStyle =
        "QScrollBar#midSessionAreaWidgetOverlayScrollBar:vertical {"
        " background: transparent;"
        " width: 12px;"
        " margin: 4px 0px 4px 0px;"
        " border: none;"
        "}"
        "QScrollBar#midSessionAreaWidgetOverlayScrollBar::handle:vertical {"
        " background: #c3c8ce;"
        " min-height: 20px;"
        " border-radius: 4px;"
        " margin: 0px 2px;"
        "}"
        "QScrollBar#midSessionAreaWidgetOverlayScrollBar::handle:vertical:hover {"
        " background: #d1d6db;"
        "}"
        "QScrollBar#midSessionAreaWidgetOverlayScrollBar::add-line:vertical,"
        "QScrollBar#midSessionAreaWidgetOverlayScrollBar::sub-line:vertical {"
        " height: 0px;"
        " border: none;"
        " background: transparent;"
        "}"
        "QScrollBar#midSessionAreaWidgetOverlayScrollBar::add-page:vertical,"
        "QScrollBar#midSessionAreaWidgetOverlayScrollBar::sub-page:vertical {"
        " background: transparent;"
        "}";
}  // namespace

MidSessionAreaWidget::MidSessionAreaWidget(QWidget *parent) : QScrollArea(parent)
{
    // 初始化资源
    this->m_sessionListWidget = new QWidget(this);                              // 创建会话列表容器
    this->m_overlayScrollBar = new QScrollBar(Qt::Vertical, this->viewport());  // 创建叠放滚动条
    // 初始化窗口
    this->_InitSessionFriendArea();

    // 初始化信号槽连接
    this->_InitSignalSlots();
}

void MidSessionAreaWidget::_InitSessionFriendArea()
{
    if (this->m_sessionListWidget == nullptr || this->m_overlayScrollBar == nullptr)
    {
        LogInfo(LogLevel::ERROR, "sessionListWidget或overlayScrollBar资源初始化失败");
        exit(-1);
    }
    // 设置背景
    this->setObjectName("midSessionAreaWidget");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->m_sessionListWidget->setObjectName("sessionListWidget");
    this->m_sessionListWidget->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet(kMidSessionAreaWidgetStyle);

    // 设置滚动区域属性,隐藏默认滚动条,使用自定义的叠放滚动条
    this->setWidgetResizable(true);  // 设置滚动区域可调整大小
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->m_overlayScrollBar->setObjectName("midSessionAreaWidgetOverlayScrollBar");
    this->m_overlayScrollBar->setFixedWidth(kOverlayScrollBarWidth);
    this->m_overlayScrollBar->setStyleSheet(kMidSessionAreaWidgetOverlayScrollBarStyle);
    this->m_overlayScrollBar->hide();
    // 将这个滚动条提升到父控件的最前面，保证它在可见时覆盖在其它控件之上。
    this->m_overlayScrollBar->raise();

    this->_SetScrollBarVisible(false);  // 开始时隐藏滚动条,按悬停状态切换显示
    this->installEventFilter(this);     // 安装事件过滤器,监听鼠标悬停事件以切换滚动条显示

    this->m_sessionListWidget->setFixedWidth(this->viewport()->width());  // 会话列表宽度跟随视口
    this->setWidget(this->m_sessionListWidget);  // 将会话列表容器设置为滚动区域的子组件

    // 布局
    QVBoxLayout *sessionListLayout = new QVBoxLayout(this->m_sessionListWidget);
    sessionListLayout->setContentsMargins(0, 0, 0, 0);  // 设置布局边距为0
    sessionListLayout->setSpacing(0);
    sessionListLayout->setAlignment(Qt::AlignTop);            // 设置布局对齐方式为顶部对齐
    this->m_sessionListWidget->setLayout(sessionListLayout);  // 将布局设置为会话列表容器的布局

    this->_UpdateOverlayScrollBarGeometry();  // 初始时更新叠放滚动条的位置和大小,确保它正确覆盖在好友列表的右侧
    this->_SyncOverlayScrollBarFromSource();

#if DEBUG_CODE
    for (int i = 0; i < 20; ++i)
    {
        this->AddItem(FriendItemType, QString("friend%1").arg(i + 1), QIcon(":/images/defaultAvatar.png"),
                      QString("好友%1").arg(i + 1), QString("最后一条消息%1").arg(i + 1));
    }
#endif
}

void MidSessionAreaWidget::_SetScrollBarVisible(bool visible)
{
    if (this->m_overlayScrollBar == nullptr) { return; }

    const bool hasScrollableContent = this->verticalScrollBar()->maximum() > this->verticalScrollBar()->minimum();
    this->m_overlayScrollBar->setVisible(visible && hasScrollableContent);
    if (visible && hasScrollableContent) { this->m_overlayScrollBar->raise(); }
}

void MidSessionAreaWidget::_UpdateOverlayScrollBarGeometry()
{
    if (this->m_overlayScrollBar == nullptr || this->viewport() == nullptr) { return; }

    int scrollBarX = this->viewport()->width() - kOverlayScrollBarWidth;
    if (scrollBarX < 0) { scrollBarX = 0; }
    this->m_overlayScrollBar->setGeometry(scrollBarX, 0, kOverlayScrollBarWidth, this->viewport()->height());
    this->m_overlayScrollBar->raise();
}

void MidSessionAreaWidget::_SyncOverlayScrollBarFromSource()
{
    if (this->m_overlayScrollBar == nullptr) { return; }

    QScrollBar *sourceScrollBar = this->verticalScrollBar();
    const bool previousSignalState = this->m_overlayScrollBar->blockSignals(true);
    this->m_overlayScrollBar->setRange(sourceScrollBar->minimum(), sourceScrollBar->maximum());
    this->m_overlayScrollBar->setPageStep(sourceScrollBar->pageStep());
    this->m_overlayScrollBar->setSingleStep(sourceScrollBar->singleStep());
    this->m_overlayScrollBar->setValue(sourceScrollBar->value());
    this->m_overlayScrollBar->blockSignals(previousSignalState);

    this->_SetScrollBarVisible(this->underMouse());
}

bool MidSessionAreaWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this)
    {
        if (event->type() == QEvent::Enter) { this->_SetScrollBarVisible(true); }
        else if (event->type() == QEvent::Leave) { this->_SetScrollBarVisible(false); }
    }

    return QScrollArea::eventFilter(watched, event);
}

void MidSessionAreaWidget::resizeEvent(QResizeEvent *event)
{
    QScrollArea::resizeEvent(event);

    if (this->m_sessionListWidget != nullptr) { this->m_sessionListWidget->setFixedWidth(this->viewport()->width()); }
    this->_UpdateOverlayScrollBarGeometry();
    this->_SyncOverlayScrollBarFromSource();
}

bool MidSessionAreaWidget::ClearFriendList()
{
    // 获取会话列表布局
    QVBoxLayout *sessionListLayout = qobject_cast<QVBoxLayout *>(this->m_sessionListWidget->layout());
    if (sessionListLayout == nullptr)
    {
        LogInfo(LogLevel::ERROR, "sessionListLayout资源获取失败");
        exit(-1);
    }

    // 删除所有会话项
    QLayoutItem *child;
    while ((child = sessionListLayout->takeAt(0)) != nullptr)
    {
        if (child->widget() != nullptr)
        {
            child->widget()->deleteLater();  // 删除会话项组件
        }
        delete child;  // 删除布局项
    }
    this->_SyncOverlayScrollBarFromSource();
    return true;
}

bool MidSessionAreaWidget::AddItem(const ItemType &type, const QString &id, const QIcon &friendIcon,
                                   const QString &friendName, const QString &lastMessage)
{
    BaseItem *item = nullptr;
    if (type == ChatItemType)
    {
        item = new ChatItem(this, id, this->m_sessionListWidget, friendIcon, friendName, lastMessage);
    }
    else if (type == FriendItemType)
    {
        item = new FriendItem(this, id, this->m_sessionListWidget, friendIcon, friendName, lastMessage);
    }
    else if (type == FriendApplyItemType)
    {
        item = new FriendApplyItem(this, id, this->m_sessionListWidget, friendIcon, friendName);
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
    sessionListLayout->addWidget(item);  // 添加项到布局
    this->_SyncOverlayScrollBarFromSource();
    return true;
}

void MidSessionAreaWidget::_InitSignalSlots()
{
    // 实现了两个滚动条的双向同步
    connect(this->verticalScrollBar(), &QScrollBar::rangeChanged, this,
            [this](int, int) { this->_SyncOverlayScrollBarFromSource(); });
    connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this->m_overlayScrollBar, &QScrollBar::setValue);
    connect(this->m_overlayScrollBar, &QScrollBar::valueChanged, this->verticalScrollBar(), &QScrollBar::setValue);
}

MidSessionAreaWidget::~MidSessionAreaWidget() = default;

// 选中特定的好友项
bool MidSessionAreaWidget::SelectFriendItem(int index)
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

    ChatItem *item = qobject_cast<ChatItem *>(layoutItem->widget());
    if (item != nullptr)
    {
        // 发送点击事件,触发选中逻辑
        QMouseEvent clickEvent(QEvent::MouseButtonPress, QPointF(1, 1), QPointF(1, 1), QPointF(1, 1), Qt::LeftButton,
                               Qt::LeftButton, Qt::NoModifier);
        QCoreApplication::sendEvent(item, &clickEvent);
    }
    return true;
}