#include <widget/self_widget/selfwidget.h>
#include <model/data.h>
#include <public.h>

using namespace ChatWidget;

namespace
{

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
    // 属性
    this->setWindowTitle("个人信息");
    this->setWindowIcon(QIcon(":/images/logo.png"));
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    this->setModal(false);
    this->setFixedSize(500, 520);
    this->setObjectName("selfWidget");
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAttribute(Qt::WA_DeleteOnClose);
}
