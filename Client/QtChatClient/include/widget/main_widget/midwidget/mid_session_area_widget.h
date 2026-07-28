#pragma once
#include <public.h>
#include <widget/main_widget/midwidget/items/chat_item.h>
#include <widget/main_widget/midwidget/items/friend_item.h>
#include <widget/main_widget/midwidget/items/friend_apply_item.h>
#include <utils/log.h>
class QResizeEvent;

namespace ChatWidget
{
    class MidSessionAreaWidget : public QScrollArea
    {
        Q_OBJECT
       private:
        void _InitSessionArea();                  // 初始化会话区域
        void _SetScrollBarVisible(bool visible);  // 按悬停状态切换滚动条显示
        void _UpdateOverlayScrollBarGeometry();   // 更新叠放滚动条的位置和大小
        void _SyncOverlayScrollBarFromSource();   // 同步真实滚动条状态到叠放滚动条

        void _InitSignalSlots();  // 初始化信号槽连接

       protected:
        bool eventFilter(QObject *watched, QEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;

       public:
        ~MidSessionAreaWidget() override;
        explicit MidSessionAreaWidget(QWidget *parent = nullptr);
        QPointer<QWidget> m_sessionListWidget;    // QScrollArea拥有，成员仅观察
        QPointer<QScrollBar> m_overlayScrollBar;  // viewport拥有，成员仅观察

        bool ClearSessionList();  // 清空会话列表

        bool AddItem(const ItemType &type, const QString &id, const QIcon &friendIcon, const QString &friendName,
                     const QString &lastMessage);  // 添加项

        // 选中特定的会话项
        bool SelectSessionItem(int index);
    };
}  // namespace ChatWidget
