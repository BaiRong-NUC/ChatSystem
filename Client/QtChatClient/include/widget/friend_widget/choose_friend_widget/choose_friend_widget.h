#pragma once

#include <public.h>
#include <widget/component/auto_hide_scroll_area.h>
#include <widget/component/search_box.h>
#include <widget/friend_widget/choose_friend_widget/choose_friend_item.h>

#include <QList>

namespace ChatWidget
{
    /**
     * 选择好友并发起群聊的窗口。
     *
     * 窗口负责好友过滤、选择状态同步和确认/取消交互，不直接创建群聊。业务层收到
     * ConfirmSelectedFriends 信号后再调用服务端接口，从而保持界面层和业务层解耦。
     */
    class ChooseFriendWidget final : public QDialog
    {
        Q_OBJECT

       public:
        struct SelectedFriendRelation
        {
            // 左右条目都由窗口中的 Qt 对象树拥有，这里只保存自动置空的观察指针。
            QPointer<ChooseFriendItem> m_sourceItem;
            QPointer<ChooseFriendItem> m_selectedItem;
        };

       private:
        void _InitChooseFriendWidget();  // 初始化选择好友窗口界面
        void _InitSignalSlots();         // 初始化搜索、完成和取消信号
        void _FilterFriends(const QString &keyword);  // 按好友名称过滤左侧列表
        void _AddSelectedFriendItem(const QIcon &icon, const QString &name,
                                    ChooseFriendItem *sourceItem = nullptr);
        void _RemoveSelectedFriendItem(ChooseFriendItem *sourceItem,
                                       ChooseFriendItem *selectedItem = nullptr);
        void _UpdateSelectedState();  // 更新数量、完成按钮和 SelectionChanged 信号

       public:
        explicit ChooseFriendWidget(QWidget *parent = nullptr);
        ~ChooseFriendWidget() override = default;

        // 在全部好友列表中加入一个好友，并可指定初始选择状态。
        void AddFriend(const QIcon &icon, const QString &name, bool isSelected = false);

        // 仅向右侧加入一个预选好友，适合业务层恢复草稿中的已有成员。
        void AddSelectedFriend(const QIcon &icon, const QString &name);

       signals:
        void SelectionChanged(const QStringList &friendNames);
        void ConfirmSelectedFriends(const QStringList &friendNames);

       public:
        QPointer<SearchBox> m_searchBox;                  // 公共搜索框组件
        QPointer<AutoHideScrollArea> m_totalFriendScrollArea;    // 左侧自动隐藏滚动区域
        QPointer<AutoHideScrollArea> m_selectedFriendScrollArea; // 右侧自动隐藏滚动区域
        QPointer<QWidget> m_totalFriendListWidget;        // 左侧全部好友列表内容区
        QPointer<QWidget> m_selectedFriendListWidget;     // 右侧已选择好友列表内容区
        QPointer<QLabel> m_selectedCountLabel;            // 已选择数量说明
        QPointer<QPushButton> m_confirmButton;             // 完成按钮
        QPointer<QPushButton> m_cancelButton;              // 取消按钮

        QList<QPointer<ChooseFriendItem>> m_friendItems;   // 左侧条目的非拥有观察列表
        QList<SelectedFriendRelation> m_selectedRelations; // 左右条目的对应关系
        QStringList m_selectedFriendNames;                 // 当前已选名称，顺序与右侧列表一致
    };
}  // namespace ChatWidget
