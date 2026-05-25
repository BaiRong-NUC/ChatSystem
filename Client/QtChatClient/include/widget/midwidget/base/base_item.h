#pragma once
#include <public.h>
#include <utils/log.h>
namespace ChatWidget
{
    enum ItemType
    {
        ChatItemType,        // 聊天项
        FriendItemType,      // 好友项
        FriendApplyItemType  // 好友申请项
    };

    class BaseItem : public QWidget
    {
        // Qt的元对象系统宏，启用信号和槽机制
        Q_OBJECT

       private:
        // 初始化好友项UI
        void _InitBaseItem(QWidget *owner, QWidget *parent = nullptr, const QIcon &icon = QIcon(),
                           const QString &name = QString(), const QString &text = QString());

       public:
        ~BaseItem() override;
        explicit BaseItem(QWidget *owner, QWidget *parent = nullptr, const QIcon &icon = QIcon(),
                          const QString &name = QString(), const QString &text = QString());

        // 左键点击事件处理
        virtual void HandleLeftClick();
        // 右键点击事件处理
        virtual void HandleRightClick();

        // 鼠标点击事件处理函数
        void mousePressEvent(QMouseEvent *event) override;

        // midwidget_area区域(多态)
        QWidget *m_midSessionAreaWidget;  // midwidget信息区域

        bool m_isSelected;  // 是否被选中

        QPushButton *m_iconButton;  // 图标按钮
        QLabel *m_nameLabel;        // 名称标签
        QLabel *m_textLabel;        // 文本标签
    };
}  // namespace ChatWidget