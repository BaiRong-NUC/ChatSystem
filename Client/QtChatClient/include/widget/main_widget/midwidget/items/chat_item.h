#pragma once
#include <public.h>
#include <utils/log.h>
#include <widget/main_widget/midwidget/base/base_item.h>
namespace ChatWidget
{
    // 好友聊天会话item
    class ChatItem : public BaseItem
    {
        Q_OBJECT
       private:
        // 当前聊天会话id
        QString m_chatSessionId;

       public:
        explicit ChatItem(QWidget *owner, const QString &chatSessionId, QWidget *parent = nullptr,
                          const QIcon &friendIcon = QIcon(), const QString &friendName = QString(),
                          const QString &text = QString());

        // 重写点击事件处理函数
        void HandleLeftClick() override;
        void HandleRightClick() override;

        ~ChatItem() override;
    };
}  // namespace ChatWidget
