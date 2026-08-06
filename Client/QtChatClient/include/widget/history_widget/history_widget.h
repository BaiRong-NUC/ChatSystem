#pragma once

#include <public.h>
#include <widget/component/auto_hide_scroll_area.h>
#include <widget/component/search_box.h>

#include <QDateEdit>
#include <QList>

/**
 * 聊天记录查询窗口。
 *
 * 当前窗口只提供两个查询条件：关键字和日期。界面层负责展示记录以及对已加载记录
 * 做本地过滤，同时发出 HistoryQueryRequested 信号；业务层后续可以监听该信号，
 * 根据会话ID从服务端分页查询，再通过 SetHistoryRecords 回填结果。
 */

namespace ChatWidget
{
    struct ChatHistoryRecord
    {
        QString m_senderName;   // 发送者显示名称
        QString m_content;      // 文本摘要；图片等消息可由业务层提供“[图片]”一类摘要
        QDateTime m_timestamp;  // 消息发送时间，用于日期过滤和时间展示
        QIcon m_senderAvatar;   // 发送者头像
    };

    class HistoryWidget final : public QWidget
    {
        Q_OBJECT

       private:
        void _InitHistoryWidget();      // 初始化标题栏、查询栏和结果列表
        void _InitSignalSlots();        // 初始化关键字、日期和窗口按钮信号
        void _RefreshHistoryResults();  // 根据当前关键字和日期重建可见结果
        void _ClearResultItems();       // 立即清空结果布局中的旧控件
        QDate _SelectedDate() const;    // 返回日期条件；无日期限制时返回无效QDate

       public:
        explicit HistoryWidget(QWidget *parent = nullptr);
        HistoryWidget(const QString &sessionName, QWidget *parent);
        ~HistoryWidget() override = default;

        void AddHistoryRecord(const ChatHistoryRecord &record);  // 追加一条聊天记录
        void SetHistoryRecords(const QList<ChatHistoryRecord> &records);  // 替换全部记录
        void ClearHistoryRecords();  // 清空当前结果数据
        void ShowCentered(QWidget *anchorWindow);  // 在指定窗口中间显示并限制到可用屏幕

       signals:
        // selectedDate无效表示查询全部日期；窗口不直接调用服务端接口。
        void HistoryQueryRequested(const QString &keyword, const QDate &selectedDate);

       public:
        QString m_sessionName;                        // 当前聊天名称
        QList<ChatHistoryRecord> m_historyRecords;    // 当前已经加载到界面的聊天记录

        QPointer<QWidget> m_titleBar;                 // 可拖动的顶部标题栏
        QPointer<QLabel> m_titleLabel;                // 会话名称和结果数量
        QPointer<QPushButton> m_minimizeButton;       // 最小化按钮
        QPointer<QPushButton> m_closeButton;          // 关闭按钮
        QPointer<SearchBox> m_searchBox;              // 公共关键字搜索框
        QPointer<QDateEdit> m_dateEdit;               // 日期选择；最小值表示全部日期
        QPointer<QPushButton> m_allDateButton;        // 将日期条件恢复为全部日期
        QPointer<AutoHideScrollArea> m_resultScrollArea; // 公共自动隐藏滚动区域
        QPointer<QWidget> m_resultContainer;          // 结果内容容器
        QPointer<QVBoxLayout> m_resultLayout;         // 聊天记录结果布局
        QPointer<QLabel> m_emptyLabel;                // 无匹配结果提示
    };
}  // namespace ChatWidget
