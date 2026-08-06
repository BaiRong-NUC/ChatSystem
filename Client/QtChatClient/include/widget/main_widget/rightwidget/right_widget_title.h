#pragma once
#include <public.h>
#include <utils/log.h>
#include <widget/session_detail_widget/single_session_detail_widget.h>
#include <widget/session_detail_widget/group_session_detai_widget.h>

namespace ChatWidget
{
    class RightWidgetTitle : public QWidget
    {
        Q_OBJECT
       private:
        void _InitRightWidgetTitle();           // 初始化右侧信息区标题栏
        void _InitSignalSlots();                // 初始化窗口控制按钮信号槽
        void _ToggleMaximized();                // 切换最大化/还原状态
        void _ToggleAlwaysOnTop(bool checked);  // 切换窗口置顶状态
        void _UpdateMaximizeButtonIcon();       // 更新最大化/还原按钮图标
       protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseDoubleClickEvent(QMouseEvent *event) override;

       public:
        explicit RightWidgetTitle(QWidget *parent = nullptr);
        ~RightWidgetTitle() override;

        // 当前会话类型；false 打开群聊详情，true 打开单聊详情。
        bool isSingleSession = false;
        QPointer<QLabel> m_titleLabel;           // 标题标签
        QPointer<QPushButton> m_titleButton;     // 更多按钮
        QPointer<QPushButton> m_pinButton;       // 窗口置顶按钮
        QPointer<QPushButton> m_minimizeButton;  // 最小化按钮
        QPointer<QPushButton> m_maximizeButton;  // 最大化/还原按钮
        QPointer<QPushButton> m_closeButton;     // 关闭按钮

        // 会话详情窗口由 Qt 父对象树管理，关闭销毁后会自动置空。
        QPointer<SingleSessionDetailWidget> m_sessionDetailWidget;

        // 群聊详情窗口由 Qt 父对象树管理，关闭销毁后会自动置空。
        QPointer<GroupSessionDetailWidget> m_groupSessionDetailWidget;
    };
}  // namespace ChatWidget
