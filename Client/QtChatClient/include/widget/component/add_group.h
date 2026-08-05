#pragma once

#include <public.h>

/**
 * 创建群聊入口组合控件。
 *
 * 当前仅提供 UI，好友选择和群聊创建逻辑由后续业务层接入。
 */

namespace ChatWidget
{
    class AddGroup final : public QWidget
    {
        Q_OBJECT

       private:
        void _InitAddGroup();  // 初始化 UI 界面

       public:
        explicit AddGroup(QWidget *parent = nullptr);
        ~AddGroup() override = default;

       public:
        QPointer<QPushButton> m_addButton;  // 创建群聊入口，当前仅展示 UI
        QPointer<QLabel> m_nameLabel;       // 入口名称
    };
}  // namespace ChatWidget
