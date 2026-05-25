#pragma once
#include <public.h>
#include <utils/log.h>
namespace ChatWidget
{
    class SessionFriendItem : public QWidget
    {
        // Qt的元对象系统宏，启用信号和槽机制
        Q_OBJECT

       private:
        // 初始化好友项UI
        void _InitSessionFriendItem(QWidget *owner, QWidget *parent = nullptr, const QIcon &friendIcon = QIcon(),
                                    const QString &friendName = QString(), const QString &text = QString());

        // 左键点击事件处理函数
        void _HandleLeftClick();
        // 右键点击事件处理函数
        void _HandleRightClick();

       public:
        ~SessionFriendItem() override;
        explicit SessionFriendItem(QWidget *owner, QWidget *parent = nullptr, const QIcon &friendIcon = QIcon(),
                                   const QString &friendName = QString(), const QString &text = QString());

        // 鼠标点击事件处理函数
        void mousePressEvent(QMouseEvent *event) override;

        // frind_area区域(多态)
        QWidget *m_friendAreaWidget;  // 好友信息区域

        bool m_isSelected;  // 是否被选中

        QPushButton *m_friendIconButton;  // 好友头像按钮
        QLabel *m_friendNameLabel;        // 好友名称标签
        QLabel *m_lastMessageLabel;       // 最后一条消息标签
    };
}  // namespace ChatWidget