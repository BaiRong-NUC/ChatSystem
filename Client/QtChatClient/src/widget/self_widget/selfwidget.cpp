#include <widget/self_widget/selfwidget.h>
#include <model/data.h>
#include <utils/log.h>
#include <public.h>

using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr auto kAvatarDefaultIconPath = ":/images/defaultAvatar.png";                  // 默认头像图标路径
    constexpr auto KAvatarStyleSheet = "border-radius: 37px; border: 1px solid #CCCCCC;";  // 头像按钮样式表
}  // namespace

SelfWidget::SelfWidget(QWidget *parent) : QDialog(parent)
{
    // 初始化原有业务控件资源
    this->m_avatarButton = new QPushButton(this);
    this->m_userIdValueLabel = new QLabel(this);
    this->m_phoneVerificationStatusLabel = new QLabel(this);
    this->m_phoneVerificationCodeTitleLabel = new QLabel(this);
    this->m_phoneVerificationCodeEdit = new QLineEdit(this);
    this->m_submitVerificationCodeButton = new QPushButton(this);
    this->m_feedbackLabel = new QLabel(this);

    this->_InitSelfWidget();
}

void SelfWidget::_InitSelfWidget()
{
    if (this->m_avatarButton == nullptr || this->m_userIdValueLabel == nullptr ||
        this->m_phoneVerificationStatusLabel == nullptr || this->m_phoneVerificationCodeTitleLabel == nullptr ||
        this->m_phoneVerificationCodeEdit == nullptr || this->m_submitVerificationCodeButton == nullptr ||
        this->m_feedbackLabel == nullptr)
    {
        LogInfo(LogLevel::ERROR, "个人信息窗口初始化失败:资源指针为nullptr");
        exit(-1);
    }
    // 属性
    this->setWindowTitle("个人信息");
    this->setWindowIcon(QIcon(":/images/logo.png"));
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    this->setModal(false);
    this->setFixedSize(500, 520);
    this->setObjectName("selfWidget");
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAttribute(Qt::WA_DeleteOnClose);

    // 1. 布局管理器
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);

    // 2. 头像
    this->m_avatarButton->setFixedSize(75, 75);
    this->m_avatarButton->setIconSize(QSize(75, 75));
    this->m_avatarButton->setIcon(QIcon(kAvatarDefaultIconPath));
    this->m_avatarButton->setStyleSheet(KAvatarStyleSheet);
    mainLayout->addWidget(this->m_avatarButton, 0, 0, 3, 1, Qt::AlignCenter);
}
