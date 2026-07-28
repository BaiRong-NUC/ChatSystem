#include <widget/self_widget/selfwidget.h>
#include <utils/log.h>
#include <public.h>

using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr auto kAvatarDefaultIconPath = ":/images/defaultAvatar.png";  // 默认头像图标路径
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
    this->setFixedSize(420, 250);
    this->setObjectName("selfWidget");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setAttribute(Qt::WA_DeleteOnClose);

    // 1. 布局管理器
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setHorizontalSpacing(10);
    mainLayout->setVerticalSpacing(7);
    mainLayout->setContentsMargins(22, 20, 22, 18);
    mainLayout->setColumnMinimumWidth(0, 88);
    mainLayout->setColumnMinimumWidth(1, 48);
    mainLayout->setColumnMinimumWidth(3, 52);
    mainLayout->setColumnStretch(2, 1);

    // 2. 头像
    this->m_avatarButton->setFixedSize(82, 82);
    this->m_avatarButton->setIconSize(QSize(78, 78));
    this->m_avatarButton->setIcon(QIcon(kAvatarDefaultIconPath));
    this->m_avatarButton->setObjectName("selfAvatarButton");
    this->m_avatarButton->setCursor(Qt::PointingHandCursor);
    mainLayout->addWidget(this->m_avatarButton, 0, 0, 4, 1, Qt::AlignTop | Qt::AlignHCenter);

    // 3. 主名称
    this->_InitEditableRow(this->m_userNameRow, "", "请输入名字", mainLayout, 0);
    this->m_userNameRow.titleLabel->hide();
    this->m_userNameRow.valueLabel->setText("未设置名字");
    this->m_userNameRow.valueLabel->setProperty("role", "primary");
    mainLayout->addWidget(this->m_userNameRow.valueLabel, 0, 1, 1, 2);
    mainLayout->addWidget(this->m_userNameRow.editor, 0, 1, 1, 2);

    // 4. 系统分配的用户ID，只展示，不提供编辑入口
    auto *userIdTitleLabel = new QLabel("ID：", this);
    userIdTitleLabel->setObjectName("selfMetaTitleLabel");
    this->m_userIdValueLabel->setText("未分配");
    this->m_userIdValueLabel->setObjectName("selfInfoValueLabel");
    this->m_userIdValueLabel->setProperty("role", "meta");
    mainLayout->addWidget(userIdTitleLabel, 1, 1);
    mainLayout->addWidget(this->m_userIdValueLabel, 1, 2, 1, 2);

    // 5. 其他可编辑资料
    this->_InitEditableRow(this->m_userTagRow, "昵称", "请输入昵称", mainLayout, 2);
    this->_InitEditableRow(this->m_signatureRow, "签名", "请输入个性签名", mainLayout, 3);

    auto *sectionDivider = new QFrame(this);
    sectionDivider->setObjectName("selfSectionDivider");
    sectionDivider->setFrameShape(QFrame::HLine);
    mainLayout->addWidget(sectionDivider, 4, 0, 1, 4);

    this->_InitEditableRow(this->m_phoneRow, "电话", "请输入电话号码", mainLayout, 5);

    // 6. 手机验证控件只完成UI准备，功能后续接入
    this->m_phoneVerificationStatusLabel->setObjectName("selfPhoneStatusLabel");
    this->m_phoneVerificationStatusLabel->setText("未验证");
    mainLayout->addWidget(this->m_phoneVerificationStatusLabel, 6, 2, 1, 2);

    this->m_phoneVerificationCodeTitleLabel->setText("验证码");
    this->m_phoneVerificationCodeTitleLabel->setObjectName("selfInfoTitleLabel");
    this->m_phoneVerificationCodeEdit->setObjectName("selfInfoEditor");
    this->m_phoneVerificationCodeEdit->setPlaceholderText("请输入验证码");
    this->m_phoneVerificationCodeEdit->setClearButtonEnabled(true);
    this->m_submitVerificationCodeButton->setText("验证");
    this->m_submitVerificationCodeButton->setObjectName("selfSubmitButton");
    mainLayout->addWidget(this->m_phoneVerificationCodeTitleLabel, 7, 1);
    mainLayout->addWidget(this->m_phoneVerificationCodeEdit, 7, 2);
    mainLayout->addWidget(this->m_submitVerificationCodeButton, 7, 3);
    this->m_phoneVerificationCodeTitleLabel->hide();
    this->m_phoneVerificationCodeEdit->hide();
    this->m_submitVerificationCodeButton->hide();

    // 7. 操作反馈区域只占位，槽函数后续接入
    this->m_feedbackLabel->setObjectName("selfFeedbackLabel");
    this->m_feedbackLabel->setAlignment(Qt::AlignCenter);
    this->m_feedbackLabel->hide();
    mainLayout->addWidget(this->m_feedbackLabel, 8, 0, 1, 4);
}

void SelfWidget::_InitEditableRow(EditableRow &editableRow, const QString &title, const QString &placeholder,
                                  QGridLayout *mainLayout, int row)
{
    if (editableRow.titleLabel == nullptr || editableRow.valueLabel == nullptr || editableRow.editor == nullptr ||
        editableRow.editButton == nullptr || editableRow.submitButton == nullptr || mainLayout == nullptr)
    {
        LogInfo(LogLevel::ERROR, "个人信息可编辑项初始化失败:资源指针为nullptr");
        exit(-1);
    }

    editableRow.titleLabel->setText(title);
    editableRow.titleLabel->setObjectName("selfInfoTitleLabel");

    editableRow.valueLabel->setObjectName("selfInfoValueLabel");
    editableRow.valueLabel->setText("未设置");

    editableRow.editor->setObjectName("selfInfoEditor");
    editableRow.editor->setPlaceholderText(placeholder);
    editableRow.editor->setClearButtonEnabled(true);
    editableRow.editor->hide();

    editableRow.editButton->setText("编辑");
    editableRow.editButton->setObjectName("selfEditButton");
    editableRow.editButton->setCursor(Qt::PointingHandCursor);

    editableRow.submitButton->setText("提交");
    editableRow.submitButton->setObjectName("selfSubmitButton");
    editableRow.submitButton->setCursor(Qt::PointingHandCursor);
    editableRow.submitButton->hide();

    mainLayout->addWidget(editableRow.titleLabel, row, 1);
    mainLayout->addWidget(editableRow.valueLabel, row, 2);
    mainLayout->addWidget(editableRow.editor, row, 2);
    mainLayout->addWidget(editableRow.editButton, row, 3);
    mainLayout->addWidget(editableRow.submitButton, row, 3);
}
