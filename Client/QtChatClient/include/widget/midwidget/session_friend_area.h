#pragma once
#include <public.h>
namespace ChatWidget
{
    class SessionFriendArea : public QWidget
    {
        Q_OBJECT
       public:
        ~SessionFriendArea() override;
        explicit SessionFriendArea(QWidget *parent = nullptr);
    };
}  // namespace ChatWidget