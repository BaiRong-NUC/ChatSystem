#include <widget/component/avatar_name.h>

using namespace ChatWidget;
using namespace Log;
using namespace Model;

namespace
{
    constexpr int kAvatarNameHeight = 100;
    constexpr int kAvatarSize = 56;
    constexpr int kAvatarRadius = 7;
    constexpr int kNameWidth = 76;
    constexpr int kNameHeight = 24;

    QIcon CreateRoundedAvatar(const QIcon &avatarIcon)
    {
        const QIcon sourceIcon = avatarIcon.isNull() ? QIcon(":/images/defaultAvatar.png") : avatarIcon;
        const QPixmap sourcePixmap = sourceIcon.pixmap(kAvatarSize, kAvatarSize);

        QPixmap roundedPixmap(kAvatarSize, kAvatarSize);
        roundedPixmap.fill(Qt::transparent);

        QPainter painter(&roundedPixmap);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

        QPainterPath clipPath;
        clipPath.addRoundedRect(roundedPixmap.rect(), kAvatarRadius, kAvatarRadius);
        painter.setClipPath(clipPath);
        painter.drawPixmap(roundedPixmap.rect(), sourcePixmap);

        return QIcon(roundedPixmap);
    }
}  // namespace

AvatarName::AvatarName(const QIcon &avatarIcon, const QString &name, QWidget *parent) : QWidget(parent)
{
    this->m_avatarButton = new QPushButton(this);
    this->m_avatarButton->setIcon(CreateRoundedAvatar(avatarIcon));
    this->m_nameLabel = new QLabel(name, this);
    this->_InitAvatarName();
}

void AvatarName::_InitAvatarName()
{
    this->setObjectName("avatarName");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setFixedSize(AVATAR_NAME_WIDTH, kAvatarNameHeight);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 7, 8, 7);
    layout->setSpacing(6);
    layout->addWidget(this->m_avatarButton, 0, Qt::AlignHCenter);
    layout->addWidget(this->m_nameLabel, 0, Qt::AlignHCenter);

    // 设置头像
    this->m_avatarButton->setObjectName("avatarButton");
    this->m_avatarButton->setFixedSize(kAvatarSize, kAvatarSize);
    this->m_avatarButton->setIconSize(QSize(kAvatarSize, kAvatarSize));
    this->m_avatarButton->setCursor(Qt::PointingHandCursor);
    this->m_avatarButton->setFocusPolicy(Qt::NoFocus);
    this->m_avatarButton->setAccessibleName(this->m_nameLabel->text());

    // 设置名字
    this->m_nameLabel->setObjectName("nameLabel");
    this->m_nameLabel->setAlignment(Qt::AlignCenter);
    this->m_nameLabel->setFont(QFont(DEFAULT_CHAT_FONT, 10));
    this->m_nameLabel->setFixedSize(kNameWidth, kNameHeight);

    // 名称超过可视宽度时使用省略号，同时保留完整名称供悬停查看。
    const QString fullName = this->m_nameLabel->text();
    const QFontMetrics fontMetrics(this->m_nameLabel->font());
    const QString displayName = fontMetrics.elidedText(fullName, Qt::ElideRight, kNameWidth);
    this->m_nameLabel->setText(displayName);
    if (displayName != fullName)
    {
        this->m_nameLabel->setToolTip(fullName);
        this->m_nameLabel->setAccessibleName(fullName);
    }
}
