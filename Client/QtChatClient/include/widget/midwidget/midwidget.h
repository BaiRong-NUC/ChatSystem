#pragma once
#include <public.h>
#include <widget/midwidget/mid_widget_title.h>
#include <widget/midwidget/mid_session_area_widget.h>
#include <utils/log.h>
namespace ChatWidget
{
    class MidWidget : public QWidget
    {
        Q_OBJECT
       private:
        explicit MidWidget(QWidget *parent = nullptr);
        static MidWidget *s_instance;  // 单例实例指针

        void _InitMidWidget();    // 初始化中间会话区
        void _InitSignalSlots();  // 初始化信号槽
       public:
        ~MidWidget() override;
        static MidWidget *GetInstance(QWidget *parent = nullptr);  // 获取单例实例的静态方法
        // 中间会话区标题
        MidWidgetTitle *m_titleBar;
        // 中间会话区会话列表
        MidSessionAreaWidget *m_sessionArea;
    };
}  // namespace ChatWidget