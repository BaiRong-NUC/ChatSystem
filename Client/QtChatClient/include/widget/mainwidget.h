#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <public.h>
#include <utils/log.h>
#include <widget/leftwidget/leftwidget.h>
#include <widget/midwidget/midwidget.h>
#include <widget/rightwidget/rightwidget.h>
using namespace Log;
using namespace ChatWidget;

class MainWidget : public QWidget
{
    Q_OBJECT
   private:
    static MainWidget *s_instance;  // 单例实例指针
    explicit MainWidget(QWidget *parent = nullptr);

    void _InitMainWidget();  // 初始化主窗口

   public:
    ~MainWidget() override;
    // 获取单例实例的静态方法
    static MainWidget *GetInstance();

    LeftWidget *m_leftWidget;    // 左侧导航栏
    MidWidget *m_midWidget;      // 中间会话区
    RightWidget *m_rightWidget;  // 右侧信息区
};
#endif  // MAINWIDGET_H
