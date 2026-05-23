#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <public.h>
#include <utils/log.h>

using namespace Log;

class MainWidget : public QWidget
{
    Q_OBJECT
   private:
    enum ActiveTab
    {
        None,             // 无标签
        SessionTab,       // 会话标签
        FriendTab,        // 好友标签
        FriendRequestTab  // 好友申请标签
    };

    static MainWidget *s_instance;  // 单例实例指针
    explicit MainWidget(QWidget *parent = nullptr);

    void _InitUI();                                           // 初始化UI界面
    void _InitMainWidget();                                   // 初始化主窗口
    void _InitLeftWidget(const ActiveTab &activeTab = None);  // 初始化左侧导航栏
    void _InitMidWidget();                                    // 初始化中间会话区
    void _InitRightWidget();                                  // 初始化右侧信息区

    void _InitSignalSlots();  // 初始化信号槽连接

    // 按钮点击事件处理函数
    // - 左侧导航栏按钮点击事件处理函数
    void _OnSessionTabClicked();                        // 会话标签按钮点击事件处理
    void _OnFriendTabClicked();                         // 好友标签按钮点击事件处理
    void _OnFriendRequestTabClicked();                  // 好友申请标签按钮点击事件处理
    void _SwitchTabButton(const ActiveTab &activeTab);  // 更新激活标签,和图标,和中间会话区

    struct TabIconInfo
    {
        const char *activeIcon;
        const char *inactiveIcon;
    };
    QHash<QPushButton *, TabIconInfo> m_tabIconInfoMap;  // 按钮到图标信息的映射

   public:
    ~MainWidget() override;
    // 获取单例实例的静态方法
    static MainWidget *GetInstance();

    QWidget *m_leftWidget;   // 左侧导航栏
    QWidget *m_midWidget;    // 中间会话区
    QWidget *m_rightWidget;  // 右侧信息区

    // 左侧导航栏
    // - 用户头像
    QPushButton *m_avatarButton;
    // - 会话标签按钮
    QPushButton *m_sessionTabButton;
    // - 好友标签按钮
    QPushButton *m_friendTabButton;
    // - 好友申请标签按钮
    QPushButton *m_friendRequestTabButton;
    // - 当前激活的标签,默认是无标签,这会影响中间会话区
    ActiveTab m_activeTab = None;
};
#endif  // MAINWIDGET_H
