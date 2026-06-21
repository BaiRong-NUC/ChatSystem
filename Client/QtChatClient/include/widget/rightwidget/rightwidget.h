#pragma once
#include <public.h>
#include <utils/log.h>
#include <widget/rightwidget/right_widget_title.h>
#include <widget/rightwidget/message_widget.h>
#include <widget/rightwidget/message_edit.h>
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
        // 上方的标题栏
        RightWidgetTitle *m_titleBar;
        // 中间的聊天窗口
        MessageWidget *m_messageWidget;
        // 下方的输入框
        MessageEdit *m_messageEdit;
    };
}  // namespace ChatWidget