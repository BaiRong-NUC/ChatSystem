#include <widget/self_widget/selfwidget.h>
#include <utils/log.h>
#include <public.h>
#include <QFrame>
#include <QMouseEvent>
#include <QStyle>

using namespace ChatWidget;
using namespace Log;
using namespace Model;

namespace
{
    constexpr auto kAvatarDefaultIconPath = ":/images/defaultAvatar.png";  // 默认头像图标路径
    constexpr qsizetype kUserIdCharactersPerLine = 24;

    QString BuildUserIdToolTip(const QString &userId)
    {
        const QString displayUserId = userId.trimmed().isEmpty() ? "未分配" : userId;
        QStringList escapedLines;
        for (qsizetype index = 0; index < displayUserId.size(); index += kUserIdCharactersPerLine)
        {
            escapedLines.append(displayUserId.mid(index, kUserIdCharactersPerLine).toHtmlEscaped());
        }

        return QString(
                   "<div style=\"white-space: nowrap;\">"
                   "<span style=\"color: #9ca3aa; font-size: 11px;\">用户 ID</span><br>"
                   "<span style=\"color: #f1f3f5; font-family: monospace;\">%1</span>"
                   "</div>")
            .arg(escapedLines.join("<br>"));
    }
}  // namespace

SelfWidget::SelfWidget(const UserInfo &userInfo, QWidget *parent) : QDialog(parent)
{
    // 初始化原有业务控件资源
    this->m_avatarButton = new QPushButton(this);
    this->m_phoneDisplayWidget = new QWidget(this);
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
               editableRow.editor == nullptr;
    };
    if (this->m_avatarButton == nullptr || this->m_phoneDisplayWidget == nullptr ||
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
    mainLayout->setColumnMinimumWidth(0, 100);
    mainLayout->setColumnMinimumWidth(1, 44);
    mainLayout->setColumnStretch(2, 1);

    // 2. 头像。用户ID通过悬停信息卡展示，避免长ID破坏主布局。
    this->m_avatarButton->setFixedSize(82, 82);
    this->m_avatarButton->setIconSize(QSize(78, 78));
    this->m_avatarButton->setIcon(userInfo.m_avatar.isNull() ? QIcon(kAvatarDefaultIconPath) : userInfo.m_avatar);
    this->m_avatarButton->setObjectName("selfAvatarButton");
    this->m_avatarButton->setCursor(Qt::PointingHandCursor);
    this->m_avatarButton->setToolTip(BuildUserIdToolTip(userInfo.m_userId));
    this->m_avatarButton->setToolTipDuration(12000);
    this->m_avatarButton->setAccessibleDescription("用户 ID：" + displayText(userInfo.m_userId, "未分配"));
    mainLayout->addWidget(this->m_avatarButton, 0, 0, 3, 1, Qt::AlignTop | Qt::AlignHCenter);

    // 3. 点击文字进入编辑状态，按回车确认。
    auto initEditableRow = [this, mainLayout, &displayText](EditableRow &editableRow, const QString &title,
                                                           const QString &value, const QString &placeholder, int row)
    {
        editableRow.titleLabel->setText(title);
        editableRow.titleLabel->setObjectName("selfInfoTitleLabel");
        editableRow.titleLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        editableRow.valueLabel->setText(displayText(value, "未设置"));
        editableRow.valueLabel->setObjectName("selfInfoValueLabel");
        editableRow.valueLabel->setProperty("editable", true);
        editableRow.valueLabel->setCursor(Qt::PointingHandCursor);
        editableRow.valueLabel->setToolTip(title.isEmpty() ? "点击修改名字" : "点击修改" + title);
        editableRow.valueLabel->installEventFilter(this);

        editableRow.editor->setText(value);
        editableRow.editor->setPlaceholderText(placeholder);
        editableRow.editor->setClearButtonEnabled(true);
        editableRow.editor->setObjectName("selfInfoEditor");
        editableRow.editor->hide();

        EditableRow *editableRowPointer = &editableRow;
        connect(editableRow.editor, &QLineEdit::returnPressed, this,
                [this, editableRowPointer]() { this->_FinishEdit(*editableRowPointer); });

        mainLayout->addWidget(editableRow.titleLabel, row, 1, Qt::AlignVCenter);
        mainLayout->addWidget(editableRow.valueLabel, row, 2);
        mainLayout->addWidget(editableRow.editor, row, 2, Qt::AlignVCenter);
    };

    // 4. 顶部主要资料
    initEditableRow(this->m_userNameRow, "", userInfo.m_userName, "请输入名字", 0);
    this->m_userNameRow.titleLabel->hide();
    this->m_userNameRow.valueLabel->setText(displayText(userInfo.m_userName, "未设置名字"));
    this->m_userNameRow.valueLabel->setProperty("role", "primary");
    mainLayout->addWidget(this->m_userNameRow.valueLabel, 0, 1, 1, 2);
    mainLayout->addWidget(this->m_userNameRow.editor, 0, 1, 1, 2);

    initEditableRow(this->m_userTagRow, "昵称", userInfo.m_userTag, "请输入昵称", 1);
    initEditableRow(this->m_signatureRow, "签名", userInfo.m_description, "请输入个性签名", 2);
    this->m_signatureRow.valueLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    this->m_signatureRow.valueLabel->setMinimumWidth(0);

    // 5. 电话资料单独分区，方便后续添加验证码交互。
    auto *sectionDivider = new QFrame(this);
    sectionDivider->setObjectName("selfSectionDivider");
    sectionDivider->setFrameShape(QFrame::HLine);
    mainLayout->addWidget(sectionDivider, 3, 0, 1, 3);

    initEditableRow(this->m_phoneRow, "电话", userInfo.m_phone, "请输入电话号码", 4);

    this->m_phoneVerificationStatusLabel->setText(userInfo.m_phoneVerified ? "已验证" : "待验证");
    this->m_phoneVerificationStatusLabel->setObjectName("selfPhoneStatusLabel");
    this->m_phoneVerificationStatusLabel->setProperty("verified", userInfo.m_phoneVerified);
    this->m_phoneVerificationStatusLabel->setToolTip(userInfo.m_phoneVerified ? "该手机号已完成验证"
                                                                             : "该手机号尚未验证");

    this->m_phoneDisplayWidget->setObjectName("selfPhoneDisplayWidget");
    auto *phoneDisplayLayout = new QHBoxLayout(this->m_phoneDisplayWidget);
    phoneDisplayLayout->setContentsMargins(0, 0, 0, 0);
    phoneDisplayLayout->setSpacing(8);
    mainLayout->removeWidget(this->m_phoneRow.valueLabel);
    phoneDisplayLayout->addWidget(this->m_phoneRow.valueLabel, 0, Qt::AlignVCenter);
    phoneDisplayLayout->addWidget(this->m_phoneVerificationStatusLabel, 0, Qt::AlignVCenter);
    phoneDisplayLayout->addStretch();
    mainLayout->addWidget(this->m_phoneDisplayWidget, 4, 2, Qt::AlignVCenter);

    // 6. 验证码和反馈控件仅预留，后续实现槽函数时再显示。
    this->m_phoneVerificationCodeTitleLabel->setText("验证码");
    this->m_phoneVerificationCodeTitleLabel->setObjectName("selfInfoTitleLabel");
    this->m_phoneVerificationCodeEdit->setPlaceholderText("请输入验证码");
    this->m_phoneVerificationCodeEdit->setClearButtonEnabled(true);
    this->m_phoneVerificationCodeEdit->setObjectName("selfInfoEditor");
    this->m_submitVerificationCodeButton->setText("验证");
    this->m_submitVerificationCodeButton->setObjectName("selfSubmitButton");

    mainLayout->addWidget(this->m_phoneVerificationCodeTitleLabel, 5, 1);
    mainLayout->addWidget(this->m_phoneVerificationCodeEdit, 5, 2);
    mainLayout->addWidget(this->m_submitVerificationCodeButton, 5, 3);
    this->m_phoneVerificationCodeTitleLabel->hide();
    this->m_phoneVerificationCodeEdit->hide();
    this->m_submitVerificationCodeButton->hide();

    this->m_feedbackLabel->setObjectName("selfFeedbackLabel");
    this->m_feedbackLabel->setAlignment(Qt::AlignCenter);
    this->m_feedbackLabel->hide();
    mainLayout->addWidget(this->m_feedbackLabel, 6, 0, 1, 4);

    this->_UpdateSignatureDisplay();
}

void SelfWidget::_BeginEdit(EditableRow &editableRow)
{
    EditableRow *allRows[] = {
        &this->m_userNameRow,
        &this->m_userTagRow,
        &this->m_signatureRow,
        &this->m_phoneRow,
    };
    for (EditableRow *otherRow : allRows)
    {
        if (otherRow != &editableRow && otherRow->editor->isVisible()) { this->_FinishEdit(*otherRow); }
    }

    editableRow.valueLabel->hide();
    if (&editableRow == &this->m_phoneRow) { this->m_phoneDisplayWidget->hide(); }
    editableRow.editor->show();
    editableRow.editor->setFocus(Qt::MouseFocusReason);
    editableRow.editor->selectAll();
}

void SelfWidget::_FinishEdit(EditableRow &editableRow)
{
    const QString value = editableRow.editor->text();
    const QString fallback = &editableRow == &this->m_userNameRow ? "未设置名字" : "未设置";
    const bool phoneChanged =
        &editableRow == &this->m_phoneRow && editableRow.valueLabel->text() != value;

    editableRow.editor->hide();
    editableRow.valueLabel->show();
    if (&editableRow == &this->m_signatureRow)
    {
        this->_UpdateSignatureDisplay();
    }
    else
    {
        editableRow.valueLabel->setText(value.trimmed().isEmpty() ? fallback : value);
    }
    if (&editableRow == &this->m_phoneRow)
    {
        if (phoneChanged)
        {
            this->m_phoneVerificationStatusLabel->setText("待验证");
            this->m_phoneVerificationStatusLabel->setProperty("verified", false);
            this->m_phoneVerificationStatusLabel->setToolTip("手机号已修改，请重新验证");
            this->m_phoneVerificationStatusLabel->style()->unpolish(this->m_phoneVerificationStatusLabel);
            this->m_phoneVerificationStatusLabel->style()->polish(this->m_phoneVerificationStatusLabel);
        }
        this->m_phoneDisplayWidget->show();
    }
}

void SelfWidget::_UpdateSignatureDisplay()
{
    if (this->m_signatureRow.valueLabel == nullptr || this->m_signatureRow.editor == nullptr) { return; }

    const QString signature = this->m_signatureRow.editor->text();
    const QString displaySignature = signature.trimmed().isEmpty() ? "未设置" : signature;
    const int availableWidth = qMax(0, this->m_signatureRow.valueLabel->contentsRect().width() - 8);
    const QString elidedSignature =
        this->m_signatureRow.valueLabel->fontMetrics().elidedText(displaySignature, Qt::ElideRight, availableWidth);
    this->m_signatureRow.valueLabel->setText(elidedSignature);
    this->m_signatureRow.valueLabel->setToolTip(elidedSignature == displaySignature ? "点击修改签名"
                                                                                   : displaySignature);
}

bool SelfWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->m_signatureRow.valueLabel && event->type() == QEvent::Resize)
    {
        this->_UpdateSignatureDisplay();
    }

    if (event->type() == QEvent::MouseButtonRelease &&
        static_cast<QMouseEvent *>(event)->button() == Qt::LeftButton)
    {
        EditableRow *allRows[] = {
            &this->m_userNameRow,
            &this->m_userTagRow,
            &this->m_signatureRow,
            &this->m_phoneRow,
        };
        for (EditableRow *editableRow : allRows)
        {
            if (watched == editableRow->valueLabel)
            {
                this->_BeginEdit(*editableRow);
                return true;
            }
        }
    }

    return QDialog::eventFilter(watched, event);
}
