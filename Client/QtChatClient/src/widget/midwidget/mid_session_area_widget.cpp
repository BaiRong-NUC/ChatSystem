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
        "QScrollBar#sessionFriendOverlayScrollBar::handle:vertical:hover {"
        " background: #d1d6db;"
        "}"
        "QScrollBar#sessionFriendOverlayScrollBar::add-line:vertical,"
        "QScrollBar#sessionFriendOverlayScrollBar::sub-line:vertical {"
        " height: 0px;"
        " border: none;"
        " background: transparent;"
        "}"
        "QScrollBar#sessionFriendOverlayScrollBar::add-page:vertical,"
        "QScrollBar#sessionFriendOverlayScrollBar::sub-page:vertical {"
        " background: transparent;"
        "}";
}  // namespace

MidSessionAreaWidget::MidSessionAreaWidget(QWidget *parent) : QScrollArea(parent)
{
    // 初始化资源
    this->m_friendListWidget = new QWidget(this);                               // 创建好友列表容器
    this->m_overlayScrollBar = new QScrollBar(Qt::Vertical, this->viewport());  // 创建叠放滚动条
    // 初始化窗口
    this->_InitSessionFriendArea();

    // 初始化信号槽连接
    this->_InitSignalSlots();
}

void MidSessionAreaWidget::_InitSessionFriendArea()
{
    if (this->m_friendListWidget == nullptr || this->m_overlayScrollBar == nullptr)
    {
        LogInfo(LogLevel::ERROR, "friendListWidget或overlayScrollBar资源初始化失败");
        exit(-1);
    }
    // 设置背景
    this->setObjectName("midSessionAreaWidget");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->m_friendListWidget->setObjectName("friendListWidget");
    this->m_friendListWidget->setAttribute(Qt::WA_StyledBackground, true);
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

    this->m_friendListWidget->setFixedWidth(this->viewport()->width());  // 好友列表宽度跟随视口
    this->setWidget(this->m_friendListWidget);  // 将好友列表容器设置为滚动区域的子组件

    // 布局
    QVBoxLayout *friendListLayout = new QVBoxLayout(this->m_friendListWidget);
    friendListLayout->setContentsMargins(0, 0, 0, 0);  // 设置布局边距为0
    friendListLayout->setSpacing(0);
    friendListLayout->setAlignment(Qt::AlignTop);           // 设置布局对齐方式为顶部对齐
    this->m_friendListWidget->setLayout(friendListLayout);  // 将布局设置为好友列表容器的布局

    this->_UpdateOverlayScrollBarGeometry();  // 初始时更新叠放滚动条的位置和大小,确保它正确覆盖在好友列表的右侧
    this->_SyncOverlayScrollBarFromSource();

#if DEBUG_CODE
    for (int i = 0; i < 20; ++i)
    {
        this->AddFriendItem(QIcon(":/images/defaultAvatar.png"), QString("好友%1").arg(i + 1),
                            QString("最后一条消息%1").arg(i + 1));
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

    if (this->m_friendListWidget != nullptr) { this->m_friendListWidget->setFixedWidth(this->viewport()->width()); }
    this->_UpdateOverlayScrollBarGeometry();
    this->_SyncOverlayScrollBarFromSource();
}

bool MidSessionAreaWidget::ClearFriendList()
{
    // 获取好友列表布局
    QVBoxLayout *friendListLayout = qobject_cast<QVBoxLayout *>(this->m_friendListWidget->layout());
    if (friendListLayout == nullptr)
    {
        LogInfo(LogLevel::ERROR, "friendListLayout资源获取失败");
        exit(-1);
    }

    // 删除所有好友项
    QLayoutItem *child;
    while ((child = friendListLayout->takeAt(0)) != nullptr)
    {
        if (child->widget() != nullptr)
        {
            child->widget()->deleteLater();  // 删除好友项组件
        }
        delete child;  // 删除布局项
    }
    this->_SyncOverlayScrollBarFromSource();
    return true;
}

bool MidSessionAreaWidget::AddFriendItem(const QIcon &friendIcon, const QString &friendName, const QString &lastMessage)
{
    // 创建好友项
    SessionFriendItem *friendItem =
        new SessionFriendItem(this, "", this->m_friendListWidget, friendIcon, friendName, lastMessage);
    if (friendItem == nullptr)
    {
        LogInfo(LogLevel::ERROR, "好友项创建失败");
        return false;
    }

    // 将好友项添加到好友列表布局
    QVBoxLayout *friendListLayout = qobject_cast<QVBoxLayout *>(this->m_friendListWidget->layout());
    if (friendListLayout == nullptr)
    {
        LogInfo(LogLevel::ERROR, "friendListLayout资源获取失败");
        exit(-1);
    }
    friendListLayout->addWidget(friendItem);  // 添加好友项到布局
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
    QVBoxLayout *friendListLayout = qobject_cast<QVBoxLayout *>(this->m_friendListWidget->layout());
    if (friendListLayout == nullptr)
    {
        LogInfo(LogLevel::ERROR, "friendListLayout资源获取失败");
        exit(-1);
    }

    int itemCount = friendListLayout->count();
    if (index < 0 || index >= itemCount)
    {
        LogInfo(LogLevel::WARNING, "选中好友项索引越界: " + QString::number(index));
        return false;
    }
    QLayoutItem *layoutItem = friendListLayout->itemAt(index);
    if (layoutItem == nullptr || layoutItem->widget() == nullptr)
    {
        LogInfo(LogLevel::ERROR, "选中好友项资源获取失败");
        return false;
    }

    SessionFriendItem *item = qobject_cast<SessionFriendItem *>(layoutItem->widget());
    if (item != nullptr)
    {
        // 发送点击事件,触发选中逻辑
        QMouseEvent clickEvent(QEvent::MouseButtonPress, QPointF(1, 1), QPointF(1, 1), QPointF(1, 1), Qt::LeftButton,
                               Qt::LeftButton, Qt::NoModifier);
        QCoreApplication::sendEvent(item, &clickEvent);
    }
    return true;
}