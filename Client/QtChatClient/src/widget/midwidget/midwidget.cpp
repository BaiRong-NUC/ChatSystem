#include <widget/midwidget/midwidget.h>
using namespace ChatWidget;

MidWidget *MidWidget::s_instance = nullptr;  // 初始化单例实例指针

MidWidget *MidWidget::GetInstance(QWidget *parent)
{
    if (s_instance == nullptr) { s_instance = new MidWidget(parent); }
    return s_instance;
}

MidWidget::MidWidget(QWidget *parent) : QWidget(parent)
{
    this->setObjectName("midWidget");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setFixedWidth(200);  // 设置中间会话区固定宽度
    this->setStyleSheet("QWidget#midWidget { background-color: #949ea9; }");
}

MidWidget::~MidWidget() = default;