#pragma once
#include <public.h>
#include <utils/log.h>
#include <widget/midwidget/base/base_item.h>
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
        ~ChatItem() override;
    };
}  // namespace ChatWidget