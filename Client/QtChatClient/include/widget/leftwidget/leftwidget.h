#pragma once
#include <public.h>
#include <utils/log.h>
namespace ChatWidget
{
    class LeftWidget : public QWidget
    {
        Q_OBJECT
       private:
        // 标签枚举,表示当前激活的标签,会影响中间会话区显示内容
        enum ActiveTab
        {
            None,             // 无标签
            SessionTab,       // 会话标签
            FriendTab,        // 好友标签
            FriendRequestTab  // 好友申请标签
        };

        // 按钮图标信息结构体,包含激活和非激活状态的图标路径
        struct TabIconInfo
        {
            const char *activeIcon;
            const char *inactiveIcon;
        };

        void _InitLeftWidget(const ActiveTab &activeTab = None);  // 初始化左侧导航栏
        // 按钮点击事件处理函数
        // - 左侧导航栏按钮点击事件处理函数
        void _OnSessionTabClicked();                        // 会话标签按钮点击事件处理
        void _OnFriendTabClicked();                         // 好友标签按钮点击事件处理
        void _OnFriendRequestTabClicked();                  // 好友申请标签按钮点击事件处理
        void _SwitchTabButton(const ActiveTab &activeTab);  // 更新激活标签,和图标,和中间会话区

        void _InitSignalSlots();  // 初始化信号槽连接

       public:
        explicit LeftWidget(QWidget *parent = nullptr);
        QHash<QPushButton *, TabIconInfo> m_tabIconInfoMap;  // 按钮到图标信息的映射
        ~LeftWidget() override;

        // 左侧导航栏
        // - 用户头像
        QPointer<QPushButton> m_avatarButton;
        // - 会话标签按钮
        QPointer<QPushButton> m_sessionTabButton;
        // - 好友标签按钮
        QPointer<QPushButton> m_friendTabButton;
        // - 好友申请标签按钮
        QPointer<QPushButton> m_friendRequestTabButton;
        // - 当前激活的标签,默认是无标签,这会影响中间会话区
        ActiveTab m_activeTab = None;
    };
}  // namespace ChatWidget
