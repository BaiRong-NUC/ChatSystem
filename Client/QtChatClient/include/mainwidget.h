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

    QWidget *m_leftWidget;   // 左侧导航栏
    QWidget *m_midWidget;    // 中间内容区
    QWidget *m_rightWidget;  // 右侧信息区

    // 用户头像
    QPushButton *m_avatarButton;
    // 会话标签按钮
    QPushButton *m_sessionTabButton;
    // 好友标签按钮
    QPushButton *m_friendTabButton;
    // 好友申请标签按钮
    QPushButton *m_friendRequestTabButton;
};
#endif  // MAINWIDGET_H
