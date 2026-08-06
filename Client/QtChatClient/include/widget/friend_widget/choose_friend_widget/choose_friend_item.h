#pragma once
#include <public.h>
#include <utils/utils.h>
#include <utils/log.h>
#include <model/data.h>

namespace ChatWidget
{
    class ChooseFriendItem : public QWidget
    {
        Q_OBJECT
       public:
        explicit ChooseFriendItem(QWidget *parent = nullptr);
        ~ChooseFriendItem() override;

        void AddFriend(const QIcon &icon, const QString &name, bool isSelected = false);

       private:
        void _InitChooseFriendItem();
        void _InitSignalSlots();

       private:
        QIcon m_icon;
        QString m_name;
        bool m_isSelected;
    };
}  // namespace ChatWidget