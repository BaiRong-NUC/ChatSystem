#pragma once

#include <public.h>

namespace ChatWidget
{
    /**
     * 选择好友窗口中的单个好友条目。
     *
     * 条目保存展示所需的头像、名称和选择状态，不承担好友数据查询。普通模式显示
     * 勾选按钮；已选模式显示移除按钮，因此左右两个列表可以复用同一套控件结构。
     */
    class ChooseFriendItem final : public QWidget
    {
        Q_OBJECT

       private:
        void _InitChooseFriendItem();  // 初始化好友条目界面
        void _ToggleSelected();        // 响应点击并切换选择状态
        void _RefreshSelectedState();  // 同步图标、动态属性和无障碍文本

       protected:
        // 点击条目空白区域时也能切换选择，交互范围与截图中的整行选择一致。
        void mouseReleaseEvent(QMouseEvent *event) override;

       public:
        explicit ChooseFriendItem(QWidget *parent = nullptr);
        ~ChooseFriendItem() override = default;

        // 填充好友信息；isSelected 用于恢复外部已有的选中状态。
        void AddFriend(const QIcon &icon, const QString &name, bool isSelected = false);
        void SetSelected(bool isSelected);  // 由外层窗口同步选择状态
        void SetRemoveMode(bool removeMode);  // 切换为右侧“已选择好友”展示模式

       signals:
        void SelectionChanged(bool isSelected);

       public:
        QIcon m_icon;
        QString m_name;
        bool m_isSelected = false;
        bool m_removeMode = false;

        // 这些子控件均带有 this 父对象，Qt 负责所有权，QPointer 仅负责安全观察。
        QPointer<QPushButton> m_selectButton;
        QPointer<QPushButton> m_avatarButton;
        QPointer<QLabel> m_nameLabel;
    };
}  // namespace ChatWidget
