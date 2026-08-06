#pragma once

#include <model/data.h>
#include <public.h>

namespace ChatWidget
{
    /**
     * 添加朋友窗口。
     *
     * 窗口只负责收集手机号或用户 ID、展示查询状态和搜索结果；实际用户查询及好友申请
     * 由业务层处理，并通过公开的状态回填接口更新界面。
     */
    class AddFriendWidget final : public QDialog
    {
        Q_OBJECT

       private:
        void _InitAddFriendWidget();
        void _InitSignalSlots();
        void _SubmitSearch();
        void _SetSearchControlsEnabled(bool enabled);
        void _ShowFeedback(const QString &message, const QString &status);
        void _UpdateResult(const Model::UserInfo &userInfo);

        bool m_dragging = false;
        QPoint m_dragOffset;
        QString m_resultUserId;
        QString m_lastSearchKeyword;

       protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;

       public:
        explicit AddFriendWidget(QWidget *parent = nullptr);
        ~AddFriendWidget() override = default;

        void setSearching(bool searching);
        void showSearchResult(const Model::UserInfo &userInfo);
        void showNoSearchResult(const QString &keyword = {});
        void showSearchError(const QString &message);
        void setFriendRequestSubmitting(bool submitting);
        void showFriendRequestResult(bool succeeded, const QString &message = {});

       signals:
        // keyword 既可以是手机号，也可以是用户 ID，由业务层按两个字段精确查询。
        void searchUserRequested(const QString &keyword);
        void addFriendRequested(const QString &userId);

       private:
        QPointer<QPushButton> m_closeButton;
        QPointer<QLineEdit> m_searchEdit;
        QPointer<QPushButton> m_searchButton;
        QPointer<QLabel> m_feedbackLabel;
        QPointer<QWidget> m_resultCard;
        QPointer<QPushButton> m_resultAvatar;
        QPointer<QLabel> m_resultName;
        QPointer<QLabel> m_resultUserIdLabel;
        QPointer<QLabel> m_resultPhoneLabel;
        QPointer<QPushButton> m_addButton;
    };
}  // namespace ChatWidget
