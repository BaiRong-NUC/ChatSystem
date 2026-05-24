#include <widget/midwidget/session_friend_area.h>

using namespace ChatWidget;

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

    // Debug
    for (int i = 0; i < 20; ++i)
    {
        QPushButton *friendButton = new QPushButton(QString("好友%1").arg(i + 1), this->m_friendListWidget);
        friendButton->setFixedHeight(40);
        friendButton->setStyleSheet(
            "QPushButton { background-color: #aeb4ba; border: none;border-radius: 5px; }"
            "QPushButton:hover { background-color: #ccd1d8; border: none;border-radius: 5px; }");
        friendListLayout->addWidget(friendButton);
    }
}

SessionFriendArea::~SessionFriendArea() = default;