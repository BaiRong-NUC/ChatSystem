#include <widget/session_detail_widget/single_session_detail_widget.h>

using namespace ChatWidget;
using namespace Log;
using namespace Model;

SingleSessionDetailWidget::SingleSessionDetailWidget(const Model::UserInfo &userInfo, QWidget *parent) : QWidget(parent)
{
    this->m_avatarName = new AvatarName(userInfo.m_avatar, userInfo.m_userName);

    this->_InitSingleSessionDetailWidget(userInfo);
}

SingleSessionDetailWidget::~SingleSessionDetailWidget() = default;

void SingleSessionDetailWidget::_InitSingleSessionDetailWidget(const Model::UserInfo &userInfo)
{
    this->setObjectName("singleSessionDetailWidget");
    this->setWindowTitle("会话详情");
    this->setWindowIcon(QIcon(":/images/logo.png"));
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setFixedSize(300, 300);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(this->m_avatarName, 0, Qt::AlignHCenter);

    this->setLayout(layout);
}