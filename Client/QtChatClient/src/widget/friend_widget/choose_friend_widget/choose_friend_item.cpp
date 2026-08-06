#include <widget/friend_widget/choose_friend_widget/choose_friend_item.h>

using namespace ChatWidget;

namespace
{
    constexpr int kFriendItemHeight = 70;
    constexpr int kSelectedItemHeight = 62;
    constexpr int kAvatarSize = 46;
    constexpr int kSelectButtonSize = 28;
    constexpr auto kDefaultAvatarPath = ":/images/defaultAvatar.png";
    constexpr auto kCheckedIconPath = ":/images/checked.png";
    constexpr auto kUncheckedIconPath = ":/images/unchecked.png";
    constexpr auto kRemoveIconPath = ":/images/icons/tabler/x.svg";
}  // namespace

ChooseFriendItem::ChooseFriendItem(QWidget *parent) : QWidget(parent)
{
    this->m_selectButton = new QPushButton(this);
    this->m_avatarButton = new QPushButton(this);
    this->m_nameLabel = new QLabel(this);

    this->_InitChooseFriendItem();
}

void ChooseFriendItem::_InitChooseFriendItem()
{
    if (this->m_selectButton == nullptr || this->m_avatarButton == nullptr || this->m_nameLabel == nullptr)
    {
        return;
    }

    this->setObjectName("chooseFriendItem");
    this->setProperty("selected", false);
    this->setProperty("mode", "friend");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setFixedHeight(kFriendItemHeight);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setCursor(Qt::PointingHandCursor);

    QHBoxLayout *itemLayout = new QHBoxLayout(this);
    itemLayout->setContentsMargins(15, 8, 15, 8);
    itemLayout->setSpacing(12);

    this->m_selectButton->setObjectName("chooseFriendSelectButton");
    this->m_selectButton->setFixedSize(kSelectButtonSize, kSelectButtonSize);
    this->m_selectButton->setIconSize(QSize(24, 24));
    this->m_selectButton->setCursor(Qt::PointingHandCursor);
    this->m_selectButton->setFocusPolicy(Qt::NoFocus);
    itemLayout->addWidget(this->m_selectButton, 0, Qt::AlignVCenter);

    this->m_avatarButton->setObjectName("chooseFriendAvatarButton");
    this->m_avatarButton->setFixedSize(kAvatarSize, kAvatarSize);
    this->m_avatarButton->setIconSize(QSize(kAvatarSize, kAvatarSize));
    this->m_avatarButton->setCursor(Qt::PointingHandCursor);
    this->m_avatarButton->setFocusPolicy(Qt::NoFocus);
    itemLayout->addWidget(this->m_avatarButton, 0, Qt::AlignVCenter);

    this->m_nameLabel->setObjectName("chooseFriendNameLabel");
    this->m_nameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    this->m_nameLabel->setTextInteractionFlags(Qt::NoTextInteraction);
    itemLayout->addWidget(this->m_nameLabel, 1, Qt::AlignVCenter);

    // 选择按钮、头像和条目空白区域都可触发同一个选择动作。
    connect(this->m_selectButton, &QPushButton::clicked, this, &ChooseFriendItem::_ToggleSelected);
    connect(this->m_avatarButton, &QPushButton::clicked, this, &ChooseFriendItem::_ToggleSelected);
    this->_RefreshSelectedState();
}

void ChooseFriendItem::_ToggleSelected()
{
    // 右侧已选条目只有“移除”语义，因此点击时固定回传 false。
    this->SetSelected(this->m_removeMode ? false : !this->m_isSelected);
}

void ChooseFriendItem::_RefreshSelectedState()
{
    if (this->m_selectButton == nullptr) { return; }

    this->setProperty("selected", this->m_isSelected);
    this->setProperty("mode", this->m_removeMode ? "selected" : "friend");

    const QString iconPath = this->m_removeMode
                                 ? QString(kRemoveIconPath)
                                 : QString(this->m_isSelected ? kCheckedIconPath : kUncheckedIconPath);
    this->m_selectButton->setIcon(QIcon(iconPath));
    this->m_selectButton->setIconSize(this->m_removeMode ? QSize(17, 17) : QSize(24, 24));
    this->m_selectButton->setAccessibleName(
        this->m_removeMode ? QStringLiteral("移除%1").arg(this->m_name)
                           : (this->m_isSelected ? QStringLiteral("取消选择%1").arg(this->m_name)
                                                : QStringLiteral("选择%1").arg(this->m_name)));

    // 动态属性变化后主动刷新，保证选中背景不需要等待下一次鼠标事件才生效。
    this->style()->unpolish(this);
    this->style()->polish(this);
    this->update();
}

void ChooseFriendItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && this->rect().contains(event->position().toPoint()))
    {
        this->_ToggleSelected();
        event->accept();
        return;
    }
    QWidget::mouseReleaseEvent(event);
}

void ChooseFriendItem::AddFriend(const QIcon &icon, const QString &name, bool isSelected)
{
    this->m_icon = icon.isNull() ? QIcon(kDefaultAvatarPath) : icon;
    this->m_name = name.trimmed().isEmpty() ? QStringLiteral("未命名好友") : name.trimmed();

    if (this->m_avatarButton != nullptr)
    {
        this->m_avatarButton->setIcon(this->m_icon);
        this->m_avatarButton->setAccessibleName(this->m_name);
    }
    if (this->m_nameLabel != nullptr)
    {
        this->m_nameLabel->setText(this->m_name);
        this->m_nameLabel->setToolTip(this->m_name);
        this->m_nameLabel->setAccessibleName(this->m_name);
    }

    this->SetSelected(isSelected);
    this->_RefreshSelectedState();
}

void ChooseFriendItem::SetSelected(bool isSelected)
{
    if (this->m_isSelected == isSelected) { return; }

    this->m_isSelected = isSelected;
    this->_RefreshSelectedState();
    emit this->SelectionChanged(this->m_isSelected);
}

bool ChooseFriendItem::IsSelected() const { return this->m_isSelected; }

void ChooseFriendItem::SetRemoveMode(bool removeMode)
{
    this->m_removeMode = removeMode;
    QHBoxLayout *itemLayout = qobject_cast<QHBoxLayout *>(this->layout());
    if (itemLayout != nullptr && this->m_selectButton != nullptr)
    {
        // 左侧好友的勾选按钮位于头像前；右侧已选好友的移除按钮放到行尾，
        // 与“头像 + 名称 + 右侧关闭按钮”的窗口视觉结构保持一致。
        itemLayout->removeWidget(this->m_selectButton);
        if (removeMode)
        {
            itemLayout->addWidget(this->m_selectButton, 0, Qt::AlignVCenter);
        }
        else
        {
            itemLayout->insertWidget(0, this->m_selectButton, 0, Qt::AlignVCenter);
        }
    }
    if (removeMode)
    {
        this->m_isSelected = true;
        this->setFixedHeight(kSelectedItemHeight);
    }
    else
    {
        this->setFixedHeight(kFriendItemHeight);
    }
    this->_RefreshSelectedState();
}

const QIcon &ChooseFriendItem::GetFriendIcon() const { return this->m_icon; }

const QString &ChooseFriendItem::GetFriendName() const { return this->m_name; }
