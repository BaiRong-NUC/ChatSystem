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
        void _InitRightWidget();  // 初始化右侧信息区
       public:
        explicit RightWidget(QWidget *parent = nullptr);
        ~RightWidget() override;
        // 上方的标题栏
        QPointer<RightWidgetTitle> m_titleBar;
        // 中间的聊天窗口
        QPointer<MessageWidget> m_messageWidget;
        // 下方的输入框
        QPointer<MessageEdit> m_messageEdit;
    };
}  // namespace ChatWidget
