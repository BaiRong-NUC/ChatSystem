#pragma once
#include <public.h>
#include <widget/component/auto_hide_scroll_area.h>
#include <widget/main_widget/midwidget/items/chat_item.h>
#include <widget/main_widget/midwidget/items/friend_item.h>
#include <widget/main_widget/midwidget/items/friend_apply_item.h>
#include <utils/log.h>
class QResizeEvent;

namespace ChatWidget
{
    class MidSessionAreaWidget : public AutoHideScrollArea
    {
        Q_OBJECT
       private:
        void _InitSessionArea();  // 初始化会话区域

       protected:
        void resizeEvent(QResizeEvent *event) override;

       public:
        ~MidSessionAreaWidget() override;
        explicit MidSessionAreaWidget(QWidget *parent = nullptr);
        QPointer<QWidget> m_sessionListWidget;    // QScrollArea拥有，成员仅观察

        bool ClearSessionList();  // 清空会话列表

        bool AddItem(const ItemType &type, const QString &id, const QIcon &friendIcon, const QString &friendName,
                     const QString &lastMessage);  // 添加项

        // 选中特定的会话项
        bool SelectSessionItem(int index);
    };
}  // namespace ChatWidget
