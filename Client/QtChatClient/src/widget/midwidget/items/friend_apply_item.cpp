#include <widget/midwidget/items/friend_apply_item.h>

using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr auto kFriendApplyAcceptButtonStyle =
        "QPushButton {"
        " background-color: #2f9e5b;"
        " color: white;"
        " border: none;"
        " border-radius: 10px;"
        " font-size: 14.5px;"
        " font-weight: 600;"
        " padding: 0 12px;"
        "}"
        "QPushButton:hover {"
        " background-color: #38b368;"
        "}"
        "QPushButton:pressed {"
        " background-color: #27854c;"
        "}";

    constexpr auto kFriendApplyRejectButtonStyle =
        "QPushButton {"
        " background-color: #333333;"
        " color: #d8d8d8;"
        " border: 1px solid #484848;"
        " border-radius: 10px;"
        " font-size: 14.5px;"
        " font-weight: 600;"
        " padding: 0 12px;"
        "}"
        "QPushButton:hover {"
        " background-color: #3d3d3d;"
        " border-color: #5a5a5a;"
        "}"
        "QPushButton:pressed {"
        " background-color: #292929;"
        "}";
}  // namespace

FriendApplyItem::FriendApplyItem(QWidget *owner, const QString &friendUserId, QWidget *parent, const QIcon &friendIcon,
                                 const QString &friendName)
    : BaseItem(owner, parent, friendIcon, friendName)
{
    // 初始化资源
    this->m_friend_user_id = friendUserId;  // 设置申请人的用户id
    this->m_acceptButton = new QPushButton("同意", this);
    this->m_rejectButton = new QPushButton("拒绝", this);
    this->m_buttonContainerWidget = new QWidget(this);  // 创建按钮容器窗口

    // 初始化UI资源
    this->_InitFriendApplyItem();
}

FriendApplyItem::~FriendApplyItem()
{
    if (this->m_acceptButton != nullptr) { delete this->m_acceptButton; }
    if (this->m_rejectButton != nullptr) { delete this->m_rejectButton; }
    if (this->m_buttonContainerWidget != nullptr) { delete this->m_buttonContainerWidget; }
    this->m_acceptButton = nullptr;
    this->m_rejectButton = nullptr;
    this->m_buttonContainerWidget = nullptr;
}

void FriendApplyItem::_InitFriendApplyItem()
{
    if (this->m_acceptButton == nullptr || this->m_rejectButton == nullptr || this->m_buttonContainerWidget == nullptr)
    {
        LogInfo(LogLevel::ERROR, "friend apply item资源初始化失败");
        exit(-1);
    }
    // 设置按钮样式
    // 高度34px，宽度根据文本内容自动调整
    this->m_acceptButton->setFixedHeight(30);
    this->m_rejectButton->setFixedHeight(30);
    this->m_acceptButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->m_rejectButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->m_acceptButton->setCursor(Qt::PointingHandCursor);
    this->m_rejectButton->setCursor(Qt::PointingHandCursor);
    this->m_acceptButton->setStyleSheet(kFriendApplyAcceptButtonStyle);
    this->m_rejectButton->setStyleSheet(kFriendApplyRejectButtonStyle);

    // 布局
    this->m_buttonContainerWidget->setFixedHeight(30);  // 设置按钮容器窗口的固定高度
    QHBoxLayout *buttonLayout = new QHBoxLayout(this->m_buttonContainerWidget);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(5);
    buttonLayout->addWidget(this->m_acceptButton);
    buttonLayout->addWidget(this->m_rejectButton);
    // 右占位(防止顶到滚动条)
    QWidget *rightPlaceholder = new QWidget(this->m_buttonContainerWidget);
    rightPlaceholder->setFixedWidth(7);
    rightPlaceholder->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    buttonLayout->addWidget(rightPlaceholder);
    this->m_buttonContainerWidget->setLayout(buttonLayout);

    // 移除多余的文本标签
    QGridLayout *mainLayout = qobject_cast<QGridLayout *>(this->layout());
    if (mainLayout == nullptr)
    {
        LogInfo(LogLevel::ERROR, "friend apply item mainLayout资源获取失败");
        exit(-1);
    }
    mainLayout->removeWidget(this->m_textLabel);
    this->m_textLabel->deleteLater();  // 删除文本标签组件
    this->m_textLabel = nullptr;

    // 将按钮容器添加到布局的第二行第二列
    mainLayout->addWidget(this->m_buttonContainerWidget, 1, 2, 1, 2);

    // 占位,防止按钮顶到边框最下方
    QWidget *bottomPlaceholder = new QWidget(this);
    bottomPlaceholder->setFixedHeight(3);
    bottomPlaceholder->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mainLayout->addWidget(bottomPlaceholder, 2, 0, 1, 4);  // 占位,横跨一行,占满所有列

    // 将名字标签文字添加居中样式
    QGridLayout *gridLayout = qobject_cast<QGridLayout *>(this->layout());
    if (gridLayout == nullptr)
    {
        LogInfo(LogLevel::ERROR, "friend apply item gridLayout资源获取失败");
        exit(-1);
    }
    QLayoutItem *nameLabelItem = gridLayout->itemAtPosition(0, 2);
    if (nameLabelItem != nullptr)
    {
        QLabel *nameLabelWidget = qobject_cast<QLabel *>(nameLabelItem->widget());
        if (nameLabelWidget != nullptr) { nameLabelWidget->setAlignment(Qt::AlignCenter); }
    }
}

void FriendApplyItem::HandleLeftClick()
{
    // 处理左键点击事件，例如同意好友申请
    LogInfo(LogLevel::INFO, "FriendApplyItem clicked: friendUserId = " + m_friend_user_id);
    // 这里可以添加同意好友申请的逻辑，例如发送同意请求给服务器
}

void FriendApplyItem::HandleRightClick()
{
    // 处理右键点击事件，例如显示上下文菜单
    LogInfo(LogLevel::INFO, "FriendApplyItem right-clicked: friendUserId = " + m_friend_user_id);
}
