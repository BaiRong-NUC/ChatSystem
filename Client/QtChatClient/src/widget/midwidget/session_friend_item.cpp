#include <widget/midwidget/session_friend_item.h>

using namespace ChatWidget;
using namespace Log;

SessionFriendItem::SessionFriendItem(QWidget *owner, QWidget *parent, const QIcon &friendIcon,
                                     const QString &friendName, const QString &text)
    : QWidget(parent)
{
    // 初始化资源
    this->m_friendAreaWidget = owner;  // (item->parent(m_friendListWidget)->parent(SessionFriendArea))
    this->m_isSelected = false;        // 默认未选中
    this->m_friendIconButton = new QPushButton(this);
    this->m_friendNameLabel = new QLabel(this);
    this->m_lastMessageLabel = new QLabel(this);

    // 初始化窗口UI资源
    this->_InitSessionFriendItem(owner, parent, friendIcon, friendName, text);
}

SessionFriendItem::~SessionFriendItem() = default;

void SessionFriendItem::_InitSessionFriendItem(QWidget *owner, QWidget *parent, const QIcon &friendIcon,
                                               const QString &friendName, const QString &text)
{
    if (this->m_friendIconButton == nullptr || this->m_friendNameLabel == nullptr ||
        this->m_lastMessageLabel == nullptr)
    {
        LogInfo(LogLevel::ERROR, "friend item 资源初始化失败");
        exit(-1);
    }

    this->setFixedHeight(60);  // 设置好友项的固定高度
    // 设置背景
    this->setObjectName("sessionFriendItem");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet(  // 949ea9;aeb4ba;ccd1d8
        "QWidget#sessionFriendItem { background-color: #949ea9; border: none; }"
        "QWidget#sessionFriendItem:hover { background-color: #aeb4ba; border: none;}");

    // 布局
    QGridLayout *friendItemLayout = new QGridLayout(this);
    friendItemLayout->setContentsMargins(10, 0, 0, 0);
    friendItemLayout->setHorizontalSpacing(10);
    friendItemLayout->setVerticalSpacing(0);
    this->setLayout(friendItemLayout);

    // 好友头像按钮
    this->m_friendIconButton->setFixedHeight(40);
    this->m_friendIconButton->setIconSize(QSize(40, 40));
    this->m_friendIconButton->setIcon(friendIcon);
    // 好友名称标签
    this->m_friendNameLabel->setText(friendName);
    this->m_friendNameLabel->setFixedHeight(30);
    // 最后一条消息标签
    this->m_lastMessageLabel->setText(text);
    this->m_lastMessageLabel->setFixedHeight(30);

    // 样式
    this->m_friendIconButton->setStyleSheet(
        "QPushButton { background-color: transparent; border: none; }"
        "QPushButton:hover { background-color: transparent; border: none; }");

    this->m_friendNameLabel->setStyleSheet("QLabel{font-size:18px;font-weight:600;}");

    this->m_friendIconButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  // 设置按钮大小策略为固定
    this->m_friendNameLabel->setSizePolicy(QSizePolicy::Expanding,
                                           QSizePolicy::Fixed);  // 设置标签大小策略为水平扩展，垂直固定
    this->m_lastMessageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 添加组件到布局
    friendItemLayout->addWidget(this->m_friendIconButton, 0, 0, 2, 2);  // 占两行两列
    friendItemLayout->addWidget(this->m_friendNameLabel, 0, 2, 1, 1);   // 第一行
    friendItemLayout->addWidget(this->m_lastMessageLabel, 1, 2, 1, 1);  // 第二行
}