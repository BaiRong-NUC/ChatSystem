#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <public.h>
#include <utils/log.h>

using namespace Log;

class MainWidget : public QWidget
{
    Q_OBJECT
   private:
    static MainWidget *s_instance;  // 单例实例指针
    explicit MainWidget(QWidget *parent = nullptr);

    void _InitUI();           // 初始化UI界面
    void _InitMainWidget();   // 初始化主窗口
    void _InitLeftWidget();   // 初始化左侧导航栏
    void _InitMidWidget();    // 初始化中间会话区
    void _InitRightWidget();  // 初始化右侧信息区

   public:
    ~MainWidget() override;
    // 获取单例实例的静态方法
    static MainWidget *GetInstance();

    QWidget *m_leftWidget;   // 左侧导航栏
    QWidget *m_midWidget;    // 中间会话区
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
