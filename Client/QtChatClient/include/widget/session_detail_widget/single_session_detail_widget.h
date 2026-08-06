#pragma once

#include <public.h>
#include <utils/log.h>
#include <model/data.h>
#include <widget/component/add_group.h>
#include <widget/component/avatar_name.h>
#include <widget/friend_widget/choose_friend_widget/choose_friend_widget.h>

/**
 * 单聊会话详情窗口
 */

namespace ChatWidget
{
    class SingleSessionDetailWidget final : public QWidget
    {
        Q_OBJECT

       private:
        void _InitSingleSessionDetailWidget(const Model::UserInfo &userInfo);  // 初始化UI界面
        void _InitSignalSlots();       // 初始化“添加”等界面信号
        void _OpenChooseFriendWidget();  // 打开选择好友窗口

        // 使用值语义保存当前单聊好友，打开选择窗口时作为默认已选成员。
        Model::UserInfo m_sessionUserInfo;

       public:
        static constexpr int PREFERRED_WIDTH = 420;  // 右侧详情栏默认宽度

        explicit SingleSessionDetailWidget(const Model::UserInfo &userInfo, QWidget *parent = nullptr);
        ~SingleSessionDetailWidget() override = default;

       signals:
        // 详情窗口仅转发建群请求，实际创建群聊由上层业务处理。
        void CreateGroupRequested(const QStringList &friendNames);

       public:
        QPointer<AvatarName> m_avatarName;                  // 用户头像+姓名组合控件
        QPointer<AddGroup> m_addGroup;                      // 创建群聊入口组合控件
        QPointer<QPushButton> m_searchMessageButton;        // 查找聊天内容入口
        QPointer<QPushButton> m_messageDoNotDisturbSwitch;  // 消息免打扰开关，当前仅展示 UI 状态
        QPointer<QPushButton> m_pinChatSwitch;              // 置顶聊天开关，当前仅展示 UI 状态
        QPointer<QPushButton> m_clearHistoryButton;         // 清空聊天记录入口
        QPointer<ChooseFriendWidget> m_chooseFriendWidget;  // 关闭详情时由 Qt 对象树一并释放
    };
}  // namespace ChatWidget
