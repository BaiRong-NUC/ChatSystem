#include <widget/user_info_widget/UserInfoWidget.h>

using namespace ChatWidget;
using namespace Log;
using namespace Model;

namespace
{
    constexpr auto kDefaultAvatarPath = ":/images/defaultAvatar.png";

    QString DisplayText(const QString &value, const QString &fallback)
    {
        return value.trimmed().isEmpty() ? fallback : value;
    }
}

UserInfoWidget::UserInfoWidget(const UserInfo &userInfo, QWidget *parent) : QDialog(parent)
{
    this->m_avatarButton = new QPushButton(this);
    this->m_userNameTag = new QLabel(this);
    this->m_userName = new QLabel(this);
    this->m_userTag = new QLabel(this);
    this->m_userTagName = new QLabel(this);
    this->m_userDescription = new QLabel(this);
    this->m_userPhone = new QLabel(this);
    this->_InitUserInfoWidget(userInfo);
}

void UserInfoWidget::_InitUserInfoWidget(const UserInfo &userInfo)
{
    if (this->m_avatarButton == nullptr || this->m_userNameTag == nullptr || this->m_userName == nullptr ||
        this->m_userTag == nullptr || this->m_userTagName == nullptr || this->m_userDescription == nullptr ||
        this->m_userPhone == nullptr)
    {
        LogInfo(LogLevel::ERROR, "聊天对象资料窗口初始化失败:资源指针为nullptr");
        return;
    }

    this->setWindowTitle("用户资料");
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    this->setModal(false);
    this->setFixedSize(390, 236);
    this->setObjectName("userInfoWidget");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet(R"(
        QDialog#userInfoWidget { background-color: #2b2b2b; border: 1px solid #484848; }
        QPushButton#userInfoAvatar { background-color: #353535; border: none; border-radius: 8px; padding: 2px; }
        QLabel#userInfoName { color: #f4f4f4; font-size: 22px; font-weight: 500; }
        QLabel#userInfoTitle { color: #a6a6a6; font-size: 13px; }
        QLabel#userInfoValue { color: #dddddd; font-size: 14px; padding-left: 4px; }
        QFrame#userInfoDivider { color: #3b3b3b; background-color: #3b3b3b; border: none; }
    )");

    this->m_avatarButton->setIcon(userInfo.m_avatar.isNull() ? QIcon(kDefaultAvatarPath) : userInfo.m_avatar);
    this->m_avatarButton->setIconSize(QSize(78, 78));
    this->m_avatarButton->setFixedSize(82, 82);
    this->m_avatarButton->setObjectName("userInfoAvatar");
    this->m_avatarButton->setEnabled(false);

    this->m_userName->setText(DisplayText(userInfo.m_userName, "未设置名字"));
    this->m_userName->setObjectName("userInfoName");
    this->m_userName->setTextInteractionFlags(Qt::TextSelectableByMouse);
    this->m_userNameTag->setText("用户 ID: " + DisplayText(userInfo.m_userId, "未分配"));
    this->m_userNameTag->setObjectName("userInfoTitle");
    this->m_userNameTag->setTextInteractionFlags(Qt::TextSelectableByMouse);

    auto initInfoRow = [](QLabel *titleLabel, QLabel *valueLabel, const QString &title, const QString &value)
    {
        titleLabel->setText(title);
        titleLabel->setObjectName("userInfoTitle");
        titleLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        valueLabel->setText(value);
        valueLabel->setObjectName("userInfoValue");
        valueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    };
    initInfoRow(this->m_userTag, this->m_userTagName, "昵称", DisplayText(userInfo.m_userTag, "未设置"));
    this->m_userDescription->setText(DisplayText(userInfo.m_description, "未设置"));
    this->m_userDescription->setObjectName("userInfoValue");
    this->m_userDescription->setWordWrap(true);
    this->m_userDescription->setTextInteractionFlags(Qt::TextSelectableByMouse);
    this->m_userPhone->setText(DisplayText(userInfo.m_phone, "未绑定"));
    this->m_userPhone->setObjectName("userInfoValue");
    this->m_userPhone->setTextInteractionFlags(Qt::TextSelectableByMouse);

    auto *layout = new QGridLayout(this);
    layout->setContentsMargins(24, 20, 24, 18);
    layout->setHorizontalSpacing(12);
    layout->setVerticalSpacing(8);
    layout->setColumnMinimumWidth(0, 82);
    layout->setColumnMinimumWidth(1, 46);
    layout->setColumnStretch(2, 1);
    layout->addWidget(this->m_avatarButton, 0, 0, 3, 1, Qt::AlignTop | Qt::AlignHCenter);
    layout->addWidget(this->m_userName, 0, 1, 1, 2);
    layout->addWidget(this->m_userNameTag, 1, 1, 1, 2);
    layout->addWidget(this->m_userTag, 2, 1);
    layout->addWidget(this->m_userTagName, 2, 2);

    auto *divider = new QFrame(this);
    divider->setObjectName("userInfoDivider");
    divider->setFrameShape(QFrame::HLine);
    layout->addWidget(divider, 3, 0, 1, 3);

    auto *descriptionTitle = new QLabel("签名", this);
    auto *phoneTitle = new QLabel("电话", this);
    for (QLabel *title : {descriptionTitle, phoneTitle})
    {
        title->setObjectName("userInfoTitle");
        title->setAlignment(Qt::AlignRight | Qt::AlignTop);
    }
    layout->addWidget(descriptionTitle, 4, 1, Qt::AlignTop);
    layout->addWidget(this->m_userDescription, 4, 2);
    layout->addWidget(phoneTitle, 5, 1);
    layout->addWidget(this->m_userPhone, 5, 2);
}
