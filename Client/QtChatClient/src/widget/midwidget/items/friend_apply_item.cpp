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
        " font-size: 16px;"
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
        " background-color: #eff2f5;"
        " color: #3c4a59;"
        " border: 1px solid #b6bec8;"
        " border-radius: 10px;"
        " font-size: 16px;"
        " font-weight: 600;"
        " padding: 0 12px;"
        "}"
        "QPushButton:hover {"
        " background-color: #e2e7ec;"
        " border-color: #9ea8b4;"
        "}"
        "QPushButton:pressed {"
        " background-color: #d5dbe2;"
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

    // 初始化UI资源
    this->_InitFriendApplyItem();
}

FriendApplyItem::~FriendApplyItem() = default;

void FriendApplyItem::_InitFriendApplyItem()
{
    if (this->m_acceptButton == nullptr || this->m_rejectButton == nullptr)
    {
        LogInfo(LogLevel::ERROR, "acceptButton或rejectButton资源初始化失败");
        exit(-1);
    }
    // 创建同意按钮
    this->m_acceptButton->setFixedSize(78, 34);  // 设置按钮大小
    this->m_rejectButton->setFixedSize(78, 34);  // 设置按钮大小
    this->m_acceptButton->setCursor(Qt::PointingHandCursor);
    this->m_rejectButton->setCursor(Qt::PointingHandCursor);
    this->m_acceptButton->setStyleSheet(kFriendApplyAcceptButtonStyle);
    this->m_rejectButton->setStyleSheet(kFriendApplyRejectButtonStyle);

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

    mainLayout->removeWidget(this->m_nameLabel);
    this->m_nameLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(this->m_nameLabel, 0, 2, 1, 2);  // 名字跨按钮区域居中

    QWidget *buttonWidget = new QWidget(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(10);
    buttonLayout->addWidget(this->m_acceptButton);
    buttonLayout->addWidget(this->m_rejectButton);
    buttonWidget->setLayout(buttonLayout);

    // 将同意和拒绝按钮添加到布局
    mainLayout->addWidget(buttonWidget, 1, 2, 1, 2, Qt::AlignCenter);  // 第二行按钮整体居中
}
