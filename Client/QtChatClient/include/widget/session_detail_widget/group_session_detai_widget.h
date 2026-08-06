#pragma once

#include <model/data.h>
#include <public.h>
#include <utils/log.h>
#include <widget/history_widget/history_widget.h>
#include <widget/component/add_group.h>
#include <widget/component/auto_hide_scroll_area.h>
#include <widget/component/avatar_name.h>
#include <widget/component/search_box.h>

/**
 * 群聊会话详情窗口。
 *
 * 当前阶段负责群成员、群资料和群设置的界面展示。群公告、折叠该聊天、关注的群成员、
 * 保存到通讯录仅保留控件，不连接服务端、不写入本地配置。
 */

namespace ChatWidget
{
    class GroupSessionDetailWidget final : public QWidget
    {
        Q_OBJECT

       private:
        void _InitGroupSessionDetailWidget();  // 初始化群聊详情界面
        void _InitSignalSlots();               // 初始化已实现的纯界面交互
        void _AddExampleMember(const QIcon &avatar, const QString &name);  // 添加调试群成员
        void _FilterGroupMembers(const QString &keyword);  // 按昵称过滤成员网格
        void _OpenHistoryWidget();      // 打开当前群聊的聊天记录窗口

       public:
        static constexpr int PREFERRED_WIDTH = 420;  // 右侧详情栏默认宽度

        explicit GroupSessionDetailWidget(const QString &groupName, QWidget *parent = nullptr);
        ~GroupSessionDetailWidget() override = default;

       public:
        QString m_groupName;                         // 当前群聊名称
        QStringList m_memberNames;                   // 群成员完整名称，与 m_memberItems 顺序一致
        QList<QPointer<AvatarName>> m_memberItems;   // Qt 对象树拥有，列表仅观察

        QPointer<AutoHideScrollArea> m_scrollArea;   // 公共自动隐藏滚动条组件
        QPointer<QWidget> m_contentWidget;           // 滚动内容容器
        QPointer<SearchBox> m_memberSearchBox;       // 公共搜索框：搜索群成员
        QPointer<QWidget> m_memberGridWidget;        // 群成员网格容器
        QPointer<AddGroup> m_addGroup;               // 添加群成员入口，当前仅展示
        QPointer<QPushButton> m_showMoreButton;      // 查看更多入口，当前仅展示

        QPointer<QLabel> m_groupNameLabel;           // 群聊名称
        QPointer<QLabel> m_groupAnnouncementLabel;   // 群公告，当前仅展示
        QPointer<QLabel> m_groupRemarkLabel;         // 群备注
        QPointer<QLabel> m_groupNicknameLabel;       // 我在本群的昵称

        QPointer<QPushButton> m_searchMessageButton;       // 查找聊天内容入口
        QPointer<QPushButton> m_messageDoNotDisturbSwitch; // 消息免打扰
        QPointer<QPushButton> m_foldChatSwitch;            // 折叠该聊天，当前仅展示
        QPointer<QPushButton> m_followMembersButton;       // 关注的群成员，当前仅展示
        QPointer<QPushButton> m_pinChatSwitch;             // 置顶聊天
        QPointer<QPushButton> m_saveToContactsSwitch;      // 保存到通讯录，当前仅展示
        QPointer<QPushButton> m_showMemberNicknameSwitch;  // 显示群成员昵称
        QPointer<QPushButton> m_clearHistoryButton;        // 清空聊天记录入口
        QPointer<QPushButton> m_exitGroupButton;           // 退出群聊入口
        QPointer<HistoryWidget> m_historyWidget;           // 关闭后自动置空的聊天记录窗口
    };
}  // namespace ChatWidget
