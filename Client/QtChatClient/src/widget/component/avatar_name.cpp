#include <widget/component/avatar_name.h>

using namespace ChatWidget;
using namespace Log;
using namespace Model;

AvatarName::AvatarName(const QIcon &avatarIcon, const QString &name, QWidget *parent) : QWidget(parent)
{
    this->m_avatarButton = new QPushButton(this);
    this->m_avatarButton->setIcon(avatarIcon);
    this->m_nameLabel = new QLabel(name, this);
    this->_InitAvatarName();
}

AvatarName::~AvatarName() = default;

void AvatarName::_InitAvatarName()
{
    this->setObjectName("avatarName");
    this->setAttribute(Qt::WA_StyledBackground, true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);
    layout->addWidget(this->m_avatarButton);
    layout->addWidget(this->m_nameLabel);

    // 设置头像
    this->m_avatarButton->setObjectName("avatarButton");
    this->m_avatarButton->setFixedSize(45, 45);
    this->m_avatarButton->setIconSize(QSize(45, 45));

    // 设置名字
    this->m_nameLabel->setObjectName("nameLabel");
    this->m_nameLabel->setAlignment(Qt::AlignCenter);
    this->m_nameLabel->setFont(QFont(DEFAULT_CHAT_FONT, 12));

    // 如果名字太长做截断
    QFontMetrics fontMetrics(this->m_nameLabel->font());
    int totalWidth = fontMetrics.horizontalAdvance(this->m_nameLabel->text());
    if (totalWidth > AVATAR_NAME_WIDTH)
    {
        //多余的部分用省略号代替
    }
}