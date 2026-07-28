#include <widget/self_widget/selfwidget.h>
#include <utils/log.h>
#include <public.h>
#include <QFrame>

using namespace ChatWidget;
using namespace Log;
using namespace Model;

namespace
{
    constexpr auto kAvatarDefaultIconPath = ":/images/defaultAvatar.png";  // 默认头像图标路径
}  // namespace

SelfWidget::SelfWidget(const UserInfo &userInfo, QWidget *parent) : QDialog(parent)
{
    // 初始化原有业务控件资源
    this->m_avatarButton = new QPushButton(this);
    this->m_userIdValueLabel = new QLabel(this);
    this->m_phoneVerificationStatusLabel = new QLabel(this);
    this->m_phoneVerificationCodeTitleLabel = new QLabel(this);
    this->m_phoneVerificationCodeEdit = new QLineEdit(this);
    this->m_submitVerificationCodeButton = new QPushButton(this);
    this->m_feedbackLabel = new QLabel(this);

    auto createEditableRow = [this](EditableRow &editableRow)
    {
        editableRow.titleLabel = new QLabel(this);
        editableRow.valueLabel = new QLabel(this);
        editableRow.editor = new QLineEdit(this);
        editableRow.editButton = new QPushButton(this);
        editableRow.submitButton = new QPushButton(this);
    };
    createEditableRow(this->m_userNameRow);
    createEditableRow(this->m_userTagRow);
    createEditableRow(this->m_signatureRow);
    createEditableRow(this->m_phoneRow);

    this->_InitSelfWidget(userInfo);
}

void SelfWidget::_InitSelfWidget(const UserInfo &userInfo)
{
    auto editableRowIsNull = [](const EditableRow &editableRow)
    {
        return editableRow.titleLabel == nullptr || editableRow.valueLabel == nullptr ||
               editableRow.editor == nullptr || editableRow.editButton == nullptr ||
               editableRow.submitButton == nullptr;
    };
    if (this->m_avatarButton == nullptr || this->m_userIdValueLabel == nullptr ||
        this->m_phoneVerificationStatusLabel == nullptr || this->m_phoneVerificationCodeTitleLabel == nullptr ||
        this->m_phoneVerificationCodeEdit == nullptr || this->m_submitVerificationCodeButton == nullptr ||
        this->m_feedbackLabel == nullptr || editableRowIsNull(this->m_userNameRow) ||
        editableRowIsNull(this->m_userTagRow) || editableRowIsNull(this->m_signatureRow) ||
        editableRowIsNull(this->m_phoneRow))
    {
        LogInfo(LogLevel::ERROR, "个人信息窗口初始化失败:资源指针为nullptr");
        exit(-1);
    }
    // 属性
    this->setWindowTitle("个人信息");
    this->setWindowIcon(QIcon(":/images/logo.png"));
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    this->setModal(false);
    this->setFixedSize(460, 250);
    this->setObjectName("selfWidget");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setAttribute(Qt::WA_DeleteOnClose);

    auto displayText = [](const QString &value, const QString &fallback)
    { return value.trimmed().isEmpty() ? fallback : value; };

    // 1. 主布局
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setHorizontalSpacing(12);
    mainLayout->setVerticalSpacing(8);
    mainLayout->setContentsMargins(24, 20, 24, 18);
    mainLayout->setColumnMinimumWidth(0, 88);
    mainLayout->setColumnMinimumWidth(1, 44);
    mainLayout->setColumnMinimumWidth(3, 44);
    mainLayout->setColumnStretch(2, 1);

    // 2. 头像
    this->m_avatarButton->setFixedSize(82, 82);
    this->m_avatarButton->setIconSize(QSize(78, 78));
    this->m_avatarButton->setIcon(userInfo.m_avatar.isNull() ? QIcon(kAvatarDefaultIconPath) : userInfo.m_avatar);
    this->m_avatarButton->setObjectName("selfAvatarButton");
    this->m_avatarButton->setCursor(Qt::PointingHandCursor);
    mainLayout->addWidget(this->m_avatarButton, 0, 0, 4, 1, Qt::AlignTop | Qt::AlignHCenter);

    // 3. 可编辑资料控件。当前只搭建UI，不连接编辑和提交槽函数。
    auto initEditableRow = [this, mainLayout, &displayText](EditableRow &editableRow, const QString &title,
                                                           const QString &value, const QString &placeholder, int row)
    {
        editableRow.titleLabel->setText(title);
        editableRow.titleLabel->setObjectName("selfInfoTitleLabel");
        editableRow.titleLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        editableRow.valueLabel->setText(displayText(value, "未设置"));
        editableRow.valueLabel->setObjectName("selfInfoValueLabel");

        editableRow.editor->setText(value);
        editableRow.editor->setPlaceholderText(placeholder);
        editableRow.editor->setClearButtonEnabled(true);
        editableRow.editor->setObjectName("selfInfoEditor");
        editableRow.editor->hide();

        editableRow.editButton->setText("修改");
        editableRow.editButton->setObjectName("selfEditButton");
        editableRow.editButton->setCursor(Qt::PointingHandCursor);
        editableRow.editButton->setToolTip(title.isEmpty() ? "修改名字" : "修改" + title);
        editableRow.editButton->setFixedSize(42, 24);

        editableRow.submitButton->setText("提交");
        editableRow.submitButton->setObjectName("selfSubmitButton");
        editableRow.submitButton->setCursor(Qt::PointingHandCursor);
        editableRow.submitButton->setFixedSize(42, 24);
        editableRow.submitButton->hide();

        mainLayout->addWidget(editableRow.titleLabel, row, 1, Qt::AlignVCenter);
        mainLayout->addWidget(editableRow.valueLabel, row, 2, Qt::AlignVCenter);
        mainLayout->addWidget(editableRow.editor, row, 2, Qt::AlignVCenter);
        mainLayout->addWidget(editableRow.editButton, row, 3, Qt::AlignRight | Qt::AlignVCenter);
        mainLayout->addWidget(editableRow.submitButton, row, 3, Qt::AlignRight | Qt::AlignVCenter);
    };

    // 4. 顶部主要资料
    initEditableRow(this->m_userNameRow, "", userInfo.m_userName, "请输入名字", 0);
    this->m_userNameRow.titleLabel->hide();
    this->m_userNameRow.valueLabel->setText(displayText(userInfo.m_userName, "未设置名字"));
    this->m_userNameRow.valueLabel->setProperty("role", "primary");
    mainLayout->addWidget(this->m_userNameRow.valueLabel, 0, 1, 1, 2);
    mainLayout->addWidget(this->m_userNameRow.editor, 0, 1, 1, 2);

    auto *userIdTitleLabel = new QLabel("ID：", this);
    userIdTitleLabel->setObjectName("selfMetaTitleLabel");
    this->m_userIdValueLabel->setText(displayText(userInfo.m_userId, "未分配"));
    this->m_userIdValueLabel->setObjectName("selfInfoValueLabel");
    this->m_userIdValueLabel->setProperty("role", "meta");
    mainLayout->addWidget(userIdTitleLabel, 1, 1);
    mainLayout->addWidget(this->m_userIdValueLabel, 1, 2, 1, 2);

    initEditableRow(this->m_userTagRow, "昵称", userInfo.m_userTag, "请输入昵称", 2);
    initEditableRow(this->m_signatureRow, "签名", userInfo.m_description, "请输入个性签名", 3);

    // 5. 电话资料单独分区，方便后续添加验证码交互。
    auto *sectionDivider = new QFrame(this);
    sectionDivider->setObjectName("selfSectionDivider");
    sectionDivider->setFrameShape(QFrame::HLine);
    mainLayout->addWidget(sectionDivider, 4, 0, 1, 4);

    initEditableRow(this->m_phoneRow, "电话", userInfo.m_phone, "请输入电话号码", 5);

    this->m_phoneVerificationStatusLabel->setText(userInfo.m_phoneVerified ? "已验证" : "待验证");
    this->m_phoneVerificationStatusLabel->setObjectName("selfPhoneStatusLabel");
    this->m_phoneVerificationStatusLabel->setProperty("verified", userInfo.m_phoneVerified);
    this->m_phoneVerificationStatusLabel->setToolTip(userInfo.m_phoneVerified ? "该手机号已完成验证"
                                                                             : "该手机号尚未验证");

    auto *phoneDisplayWidget = new QWidget(this);
    phoneDisplayWidget->setObjectName("selfPhoneDisplayWidget");
    auto *phoneDisplayLayout = new QHBoxLayout(phoneDisplayWidget);
    phoneDisplayLayout->setContentsMargins(0, 0, 0, 0);
    phoneDisplayLayout->setSpacing(8);
    mainLayout->removeWidget(this->m_phoneRow.valueLabel);
    phoneDisplayLayout->addWidget(this->m_phoneRow.valueLabel, 0, Qt::AlignVCenter);
    phoneDisplayLayout->addWidget(this->m_phoneVerificationStatusLabel, 0, Qt::AlignVCenter);
    phoneDisplayLayout->addStretch();
    mainLayout->addWidget(phoneDisplayWidget, 5, 2, Qt::AlignVCenter);

    // 6. 验证码和反馈控件仅预留，后续实现槽函数时再显示。
    this->m_phoneVerificationCodeTitleLabel->setText("验证码");
    this->m_phoneVerificationCodeTitleLabel->setObjectName("selfInfoTitleLabel");
    this->m_phoneVerificationCodeEdit->setPlaceholderText("请输入验证码");
    this->m_phoneVerificationCodeEdit->setClearButtonEnabled(true);
    this->m_phoneVerificationCodeEdit->setObjectName("selfInfoEditor");
    this->m_submitVerificationCodeButton->setText("验证");
    this->m_submitVerificationCodeButton->setObjectName("selfSubmitButton");

    mainLayout->addWidget(this->m_phoneVerificationCodeTitleLabel, 7, 1);
    mainLayout->addWidget(this->m_phoneVerificationCodeEdit, 7, 2);
    mainLayout->addWidget(this->m_submitVerificationCodeButton, 7, 3);
    this->m_phoneVerificationCodeTitleLabel->hide();
    this->m_phoneVerificationCodeEdit->hide();
    this->m_submitVerificationCodeButton->hide();

    this->m_feedbackLabel->setObjectName("selfFeedbackLabel");
    this->m_feedbackLabel->setAlignment(Qt::AlignCenter);
    this->m_feedbackLabel->hide();
    mainLayout->addWidget(this->m_feedbackLabel, 8, 0, 1, 4);
}
