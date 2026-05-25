#pragma once
#include <public.h>
#include <widget/midwidget/items/chat_item.h>
#include <utils/log.h>
class QResizeEvent;

namespace ChatWidget
{
    class MidSessionAreaWidget : public QScrollArea
    {
        Q_OBJECT
       private:
        void _InitSessionFriendArea();            // 初始化好友信息区域
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
        QWidget *m_friendListWidget;     // 好友列表容器
        QScrollBar *m_overlayScrollBar;  // 叠放在好友列表上的竖向滚动条

        bool ClearFriendList();  // 清空好友列表

        bool AddFriendItem(const QIcon &friendIcon, const QString &friendName,
                           const QString &lastMessage);  // 添加好友项

        // 选中特定的好友项
        bool SelectFriendItem(int index);
    };
}  // namespace ChatWidget