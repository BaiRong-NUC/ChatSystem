#pragma once
#include <public.h>
#include <utils/log.h>

namespace ChatWidget
{
    class MidWidgetTitle : public QWidget
    {
        Q_OBJECT
       private:
        explicit MidWidgetTitle(QWidget *parent = nullptr);
        static MidWidgetTitle *s_instance;  // 单例实例指针

        void _InitMidWidgetTitle();  // 初始化标题栏

       public:
        ~MidWidgetTitle() override;
        static MidWidgetTitle *GetInstance(QWidget *parent = nullptr);  // 获取单例实例的静态方法

        // 按钮
        QPushButton *m_addFriendButton;
        // 搜索框
        QLineEdit *m_searchLineEdit;
        // Icon
        QString m_addFriendButtonIconPath;
    };
}  // namespace ChatWidget