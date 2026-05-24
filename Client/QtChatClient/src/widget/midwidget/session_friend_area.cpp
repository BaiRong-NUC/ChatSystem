#include <widget/midwidget/session_friend_area.h>

using namespace ChatWidget;
using namespace Log;

SessionFriendArea::SessionFriendArea(QWidget *parent) : QScrollArea(parent)
{
    // 初始化窗口
    this->_InitSessionFriendArea();
}

void SessionFriendArea::_InitSessionFriendArea()
{
    // 设置滚动区域属性
    this->setWidgetResizable(true);  // 设置滚动区域可调整大小
    // 设置滚动条样式
    this->verticalScrollBar()->setStyleSheet(
        "QScrollBar:vertical { background: #aeb4ba; width: 10px; margin: 0px 0px 0px 0px; }");

    // 设置水平滚动条样式
    this->horizontalScrollBar()->setStyleSheet(
        "QScrollBar:horizontal { background: #aeb4ba; height: 0px; margin: 0px 0px 0px 0px; }");
    this->m_friendListWidget = new QWidget(this);  // 创建好友列表容器
    this->m_friendListWidget->setFixedWidth(200);  // 设置好友列表容器的固定宽度
    this->setWidget(this->m_friendListWidget);     // 将好友列表容器设置为滚动区域的子组件

    // 布局
    QVBoxLayout *friendListLayout = new QVBoxLayout(this->m_friendListWidget);
    friendListLayout->setContentsMargins(0, 0, 0, 0);  // 设置布局边距为0
    friendListLayout->setSpacing(0);
    friendListLayout->setAlignment(Qt::AlignTop);  // 设置布局对齐方式为顶部对齐

    this->m_friendListWidget->setLayout(friendListLayout);  // 将布局设置为好友列表容器的布局

#if DEBUG_CODE
    for (int i = 0; i < 20; ++i)
    {
        this->AddFriendItem(QIcon(":/images/defaultAvatar.png"), QString("好友%1").arg(i + 1),
                            QString("最后一条消息%1").arg(i + 1));
    }
#endif
}

bool SessionFriendArea::ClearFriendList()
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
    return true;
}

bool SessionFriendArea::AddFriendItem(const QIcon &friendIcon, const QString &friendName, const QString &lastMessage)
{
    // 创建好友项
    SessionFriendItem *friendItem =
        new SessionFriendItem(this->m_friendListWidget, this->m_friendListWidget, friendIcon, friendName, lastMessage);
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
    return true;
}

SessionFriendArea::~SessionFriendArea() = default;