#include <widget/self_widget/selfwidget.h>

#include <model/data.h>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStyle>
#include <QVBoxLayout>

using namespace ChatWidget;

SelfWidget::SelfWidget(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle("个人信息");
    this->setWindowIcon(QIcon(":/images/logo.png"));
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    this->setModal(true);
    this->setFixedSize(450, 250);
    this->setAttribute(Qt::WA_DeleteOnClose);  // 确保对话框关闭时自动删除对象，避免内存泄漏
}
