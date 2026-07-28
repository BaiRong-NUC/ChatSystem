#pragma once
#include <utils/utils.h>
#include <utils/log.h>
#include <model/data.h>

namespace ChatWidget
{
    // 根据消息类型创建不同的消息内容控件,并添加到布局中
    class ChatMessage : public QWidget
    {
        Q_OBJECT
       private:
        // 初始化UI
        void _InitChatMessage();

        void _MakeTextMessage();    // 创建文本消息内容控件
        void _MakeImageMessage();   // 创建图片消息内容控件
        void _MakeFileMessage();    // 创建文件消息内容控件
        void _MakeSpeechMessage();  // 创建语音消息内容控件

       public:
        explicit ChatMessage(const Model::Message &message, bool isLeft = true,
                             const QFont &textFont = QFont(DEFAULT_CHAT_FONT, 14), QWidget *parent = nullptr);
        ~ChatMessage() override;

        std::unique_ptr<Model::Message> m_message;
        QFont m_textFont;
        bool m_isLeft = true;
    };

    // 文本消息展示
    class TextChatMessage : public QWidget
    {
        Q_OBJECT

       public:
        explicit TextChatMessage(const QString &text, bool isLeft,
                                 const QFont &textFont = QFont(DEFAULT_CHAT_FONT, 14),
                                 QWidget *parent = nullptr);
        ~TextChatMessage() override;
        QFont m_textFont;               // 文本消息字体
        QPointer<QLabel> m_textLabel;  // Qt父对象拥有，成员仅观察
        bool m_isLeft = true;           // 是否是左侧消息,左侧消息和右侧消息不同

        // 控件显示时,重写绘制事件,绘制不规则对话框背景
        void paintEvent(QPaintEvent *event) override;
    };
}  // namespace ChatWidget
