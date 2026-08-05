#pragma once
#include <public.h>
#include <utils/log.h>
#include <widget/main_widget/leftwidget/leftwidget.h>
#include <widget/main_widget/midwidget/midwidget.h>
#include <widget/main_widget/rightwidget/rightwidget.h>
#include <widget/friend_widget/add_friend_widget.h>

namespace ChatWidget
{
    class MainWidget : public QWidget
    {
        Q_OBJECT
       private:
        void _InitMainWidget();  // 初始化主窗口
        void _OpenAddFriendWidget();

       public:
        explicit MainWidget(QWidget *parent = nullptr);
        ~MainWidget() override;

        void showUserSearchResult(const Model::UserInfo &userInfo);
        void showNoUserSearchResult(const QString &keyword = {});
        void showUserSearchError(const QString &message);
        void showFriendRequestResult(bool succeeded, const QString &message = {});

       signals:
        void searchUserRequested(const QString &keyword);
        void addFriendRequested(const QString &userId);

       public:
        QPointer<LeftWidget> m_leftWidget;    // Qt父对象拥有，成员仅观察
        QPointer<MidWidget> m_midWidget;      // Qt父对象拥有，成员仅观察
        QPointer<RightWidget> m_rightWidget;  // Qt父对象拥有，成员仅观察
        QPointer<AddFriendWidget> m_addFriendWidget;  // 关闭销毁后自动置空
    };
}  // namespace ChatWidget
