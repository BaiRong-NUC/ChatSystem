#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <public.h>

class MainWidget : public QWidget
{
    Q_OBJECT
   private:
    static MainWidget *s_instance;  // 单例实例指针
    explicit MainWidget(QWidget *parent = nullptr);

   public:
    ~MainWidget() override;
    // 获取单例实例的静态方法
    static MainWidget *GetInstance();
};
#endif  // MAINWIDGET_H
