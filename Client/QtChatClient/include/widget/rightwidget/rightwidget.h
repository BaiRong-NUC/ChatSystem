#pragma once
#include <public.h>
#include <utils/log.h>
#include <widget/rightwidget/message_widget.h>
namespace ChatWidget
{
    class RightWidget : public QWidget
    {
        Q_OBJECT
       private:
        explicit RightWidget(QWidget *parent = nullptr);
        void _InitRightWidget();         // 初始化右侧信息区
        static RightWidget *s_instance;  // 单例实例指针
       public:
        ~RightWidget() override;
        static RightWidget *GetInstance(QWidget *parent = nullptr);  // 获取单例实例的静态方法

        QWidget *m_titleBar;         // 上方标题栏
        QLabel *m_titleLabel;        // 标题标签
        QPushButton *m_titleButton;  // 标题按钮

        // 中间的聊天窗口
        MessageWidget *m_messageWidget;
    };
}  // namespace ChatWidget