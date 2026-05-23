#pragma once
#include <public.h>
namespace ChatWidget
{
    class MidWidget : public QWidget
    {
        Q_OBJECT
       private:
        explicit MidWidget(QWidget *parent = nullptr);
        // void _InitMidWidget();         // 初始化中间会话区
        static MidWidget *s_instance;  // 单例实例指针
       public:
        ~MidWidget() override;
        static MidWidget *GetInstance(QWidget *parent = nullptr);  // 获取单例实例的静态方法
        // 中间会话区
        // - 搜索框
        QLineEdit *m_searchLineEdit;
        // - 添加好友按钮
        QPushButton *m_addFriendButton;
        const char *m_addFriendButtonIconPath;
    };
}  // namespace ChatWidget