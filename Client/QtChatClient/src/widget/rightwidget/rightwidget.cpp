#include <widget/rightwidget/rightwidget.h>
using namespace ChatWidget;

RightWidget *RightWidget::s_instance = nullptr;  // 初始化单例实例指针

RightWidget *RightWidget::GetInstance(QWidget *parent)
{
    if (s_instance == nullptr) { s_instance = new RightWidget(parent); }
    return s_instance;
}

RightWidget::RightWidget(QWidget *parent) : QWidget(parent)
{
    this->setObjectName("rightWidget");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setMinimumWidth(350);  // 设置右侧信息区最小高度,宽度,根据窗口大小自动调整
    this->setMinimumHeight(350);
    this->setStyleSheet("QWidget#rightWidget { background-color: #ecf0f1; }");
}

RightWidget::~RightWidget() = default;