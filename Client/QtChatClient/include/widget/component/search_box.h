#pragma once

#include <public.h>

/**
 * 通用搜索框。
 *
 * 该控件只负责搜索输入相关的通用交互：展示搜索图标、输入关键词、清空内容，
 * 并在文本变化或用户按下回车时发出信号。具体搜索对象和过滤规则由使用方决定，
 * 因而好友、会话、聊天记录等页面都可以复用同一个控件。
 */

namespace ChatWidget
{
    class SearchBox final : public QWidget
    {
        Q_OBJECT

       private:
        void _InitSearchBox();  // 初始化通用搜索框界面
        void _RefreshFocusStyle(bool focused);  // 刷新获得/失去焦点时的动态样式

       protected:
        // 监听内部输入框的焦点事件，使外层边框也能响应焦点状态。
        bool eventFilter(QObject *watched, QEvent *event) override;

       public:
        explicit SearchBox(QWidget *parent = nullptr);
        ~SearchBox() override = default;

        void SetPlaceholderText(const QString &placeholderText);  // 设置占位提示
        void SetKeyword(const QString &keyword);                  // 设置当前关键词
        QString GetKeyword() const;                               // 获取去除首尾空格后的关键词
        void Clear();                                             // 清空关键词
        void SetSearchFocus();                                    // 将键盘焦点交给输入框

       signals:
        void KeywordChanged(const QString &keyword);  // 输入内容变化时实时通知使用方
        void SearchRequested(const QString &keyword); // 用户按下回车时请求执行搜索

       private:
        // 子控件拥有明确的 QObject 父对象，由 Qt 对象树释放；成员仅作为安全观察指针。
        QPointer<QLineEdit> m_searchEdit;
    };
}  // namespace ChatWidget
