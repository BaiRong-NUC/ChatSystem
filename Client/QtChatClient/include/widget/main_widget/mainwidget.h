#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <public.h>
#include <utils/log.h>
#include <widget/main_widget/leftwidget/leftwidget.h>
#include <widget/main_widget/midwidget/midwidget.h>
#include <widget/main_widget/rightwidget/rightwidget.h>
using namespace Log;
using namespace ChatWidget;

class MainWidget : public QWidget
{
    Q_OBJECT
   private:
    void _InitMainWidget();  // 初始化主窗口

   public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget() override;

    QPointer<LeftWidget> m_leftWidget;    // Qt父对象拥有，成员仅观察
    QPointer<MidWidget> m_midWidget;      // Qt父对象拥有，成员仅观察
    QPointer<RightWidget> m_rightWidget;  // Qt父对象拥有，成员仅观察
};
#endif  // MAINWIDGET_H
