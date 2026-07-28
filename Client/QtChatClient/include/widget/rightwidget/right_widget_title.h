#pragma once
#include <public.h>
#include <utils/log.h>

namespace ChatWidget
{
    class RightWidgetTitle : public QWidget
    {
        Q_OBJECT
       private:
        explicit RightWidgetTitle(QWidget *parent = nullptr);
        void _InitRightWidgetTitle();         // 初始化右侧信息区标题栏
        void _InitSignalSlots();              // 初始化窗口控制按钮信号槽
        void _ToggleMaximized();              // 切换最大化/还原状态
        void _ToggleAlwaysOnTop(bool checked);  // 切换窗口置顶状态
        void _UpdateMaximizeButtonIcon();     // 更新最大化/还原按钮图标
        static RightWidgetTitle *s_instance;  // 单例实例指针

       protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseDoubleClickEvent(QMouseEvent *event) override;

       public:
        ~RightWidgetTitle() override;
        static RightWidgetTitle *GetInstance(QWidget *parent = nullptr);  // 获取单例实例的静态方法

        QLabel *m_titleLabel;              // 标题标签
        QPushButton *m_titleButton;        // 更多按钮
        QPushButton *m_pinButton;          // 窗口置顶按钮
        QPushButton *m_minimizeButton;     // 最小化按钮
        QPushButton *m_maximizeButton;     // 最大化/还原按钮
        QPushButton *m_closeButton;        // 关闭按钮
    };
}  // namespace ChatWidget
