#pragma once
#include <public.h>
#include <widget/main_widget/midwidget/mid_widget_title.h>
#include <widget/main_widget/midwidget/mid_session_area_widget.h>
#include <utils/log.h>
namespace ChatWidget
{
    class MidWidget : public QWidget
    {
        Q_OBJECT
       private:
        void _InitMidWidget();    // 初始化中间会话区
        void _InitSignalSlots();  // 初始化信号槽
       public:
        explicit MidWidget(QWidget *parent = nullptr);
        ~MidWidget() override;

       signals:
        void addFriendRequested();

       public:
        // 中间会话区标题
        QPointer<MidWidgetTitle> m_titleBar;
        // 中间会话区会话列表
        QPointer<MidSessionAreaWidget> m_sessionArea;
    };
}  // namespace ChatWidget
