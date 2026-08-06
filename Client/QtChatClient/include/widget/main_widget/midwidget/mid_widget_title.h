#pragma once
#include <public.h>
#include <utils/log.h>

namespace ChatWidget
{
    class MidWidgetTitle : public QWidget
    {
        Q_OBJECT
       private:
        void _InitMidWidgetTitle();  // 初始化标题栏

        void _InitSignalSlots();  // 初始化信号槽

       public:
        explicit MidWidgetTitle(QWidget *parent = nullptr);
        ~MidWidgetTitle() override;

       signals:
        void addFriendRequested();

       public:
        // 按钮
        QPointer<QPushButton> m_addFriendButton;
        // 搜索框
        QPointer<QLineEdit> m_searchLineEdit;
    };
}  // namespace ChatWidget
