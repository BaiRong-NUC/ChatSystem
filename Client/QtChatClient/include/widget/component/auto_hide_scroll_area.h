#pragma once

#include <public.h>

class QResizeEvent;
class QShowEvent;

/**
 * 自动隐藏滚动条的公共滚动区域。
 *
 * QScrollArea 自带的滚动条在不同系统上可能显示轨道、上下箭头等原生装饰。
 * 本组件隐藏原生滚动条，只保留它的滚动范围和值，再在 viewport 右侧叠放一个
 * 统一样式的滚动条：鼠标进入控件时显示，离开时隐藏。
 *
 * 使用方式：
 * 1. 需要自定义滚动控件时继承 AutoHideScrollArea；
 * 2. 只需要普通滚动容器时，直接创建 AutoHideScrollArea 并调用 setWidget；
 * 3. 内容数量被同步修改后，可调用 RefreshScrollBar 立即刷新状态。
 */

namespace ChatWidget
{
    class AutoHideScrollArea : public QScrollArea
    {
        Q_OBJECT

       private:
        void _InitAutoHideScrollArea();             // 初始化隐藏策略和信号连接
        void _SetScrollBarVisible(bool visible);    // 根据悬停状态切换覆盖滚动条
        void _UpdateOverlayScrollBarGeometry();     // 将覆盖滚动条固定在viewport右侧
        void _SyncOverlayScrollBarFromSource();     // 从隐藏的真实滚动条同步范围与位置

       protected:
        bool eventFilter(QObject *watched, QEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;
        void showEvent(QShowEvent *event) override;

       public:
        explicit AutoHideScrollArea(QWidget *parent = nullptr);
        ~AutoHideScrollArea() override = default;

        // 内容布局刚发生同步变化时，调用该方法可以立即更新滑块大小与可见性。
        void RefreshScrollBar();

       public:
        // 覆盖滚动条由viewport的Qt对象树拥有；成员仅作自动置空的观察指针。
        QPointer<QScrollBar> m_overlayScrollBar;
    };
}  // namespace ChatWidget
