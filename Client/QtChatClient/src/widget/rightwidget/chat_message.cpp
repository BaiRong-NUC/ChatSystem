#include <widget/rightwidget/chat_message.h>

using namespace ChatWidget;
using namespace Model;
using namespace Log;

namespace
{
    constexpr auto kLeftTextLabelStyle =
        "QLabel { color: #f2f2f2; background: transparent; font-size: 12px; }";
    constexpr auto kRightTextLabelStyle =
        "QLabel { color: #111111; background: transparent; font-size: 12px; }";
}

ChatMessage::ChatMessage(const Model::Message &message, bool isLeft, const QFont &textFont, QWidget *parent)
    : QWidget(parent), m_message(std::make_unique<Model::Message>(message)), m_textFont(textFont), m_isLeft(isLeft)
{
    // 初始化UI
    this->_InitChatMessage();
}
ChatMessage::~ChatMessage() = default;

void ChatMessage::_InitChatMessage()
{
    if (this->m_message == nullptr)
    {
        LogInfo(LogLevel::ERROR, "消息内容控件资源初始化失败");
        exit(-1);
    }
    // 初始化UI,根据不同的消息类型做处理
    switch (this->m_message->m_messageType)
    {
        case Model::MessageType::Text:
            this->_MakeTextMessage();
            break;
        case Model::MessageType::Image:
            this->_MakeImageMessage();
            break;
        case Model::MessageType::File:
            this->_MakeFileMessage();
            break;
        case Model::MessageType::SPEECH:
            this->_MakeSpeechMessage();
            break;
        default:
            LogInfo(LogLevel::ERROR, "未知的消息类型");
            exit(-1);
    }
}

/**
 *
 *
 *                          一. 初始化文本类型消息
 *
 *
 */
// 重写绘制事件,绘制不规则对话框背景
void TextChatMessage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    constexpr int arrowWidth = 9;
    constexpr int radius = 7;
    const QColor bubbleColor = this->m_isLeft ? QColor("#363638") : QColor("#43c780");

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(bubbleColor);

    QRectF bubbleRect;
    QPainterPath tail;
    if (this->m_isLeft)
    {
        bubbleRect = QRectF(arrowWidth, 0, width() - arrowWidth, height());
        tail.moveTo(arrowWidth + 1, 12);
        tail.lineTo(0, 19);
        tail.lineTo(arrowWidth + 1, 26);
    }
    else
    {
        bubbleRect = QRectF(0, 0, width() - arrowWidth, height());
        tail.moveTo(width() - arrowWidth - 1, 12);
        tail.lineTo(width(), 19);
        tail.lineTo(width() - arrowWidth - 1, 26);
    }
    tail.closeSubpath();
    painter.drawRoundedRect(bubbleRect, radius, radius);
    painter.drawPath(tail);
}

TextChatMessage::TextChatMessage(const QString &text, bool isLeft, const QFont &textFont, QWidget *parent)
    : QWidget(parent), m_textFont(textFont), m_isLeft(isLeft)
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    this->setMinimumHeight(44);
    this->setMaximumWidth(500);

    this->m_textLabel = new QLabel(this);
    this->m_textLabel->setText(text);
    this->m_textLabel->setFont(this->m_textFont);
    this->m_textLabel->setWordWrap(true);
    this->m_textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    this->m_textLabel->setStyleSheet(this->m_isLeft ? kLeftTextLabelStyle : kRightTextLabelStyle);

    QHBoxLayout *layout = new QHBoxLayout(this);
    if (this->m_isLeft)
    {
        layout->setContentsMargins(21, 10, 13, 10);
    }
    else
    {
        layout->setContentsMargins(13, 10, 21, 10);
    }
    layout->addWidget(this->m_textLabel);
}

TextChatMessage::~TextChatMessage() = default;

void ChatMessage::_MakeTextMessage()
{
    auto textMessage = std::make_unique<TextChatMessage>(
        QString::fromUtf8(this->m_message->m_content), this->m_isLeft, this->m_textFont, this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(textMessage.release());  // 所有权交给ChatMessage的Qt对象树
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
}

void ChatMessage::_MakeImageMessage() {}

void ChatMessage::_MakeFileMessage() {}

void ChatMessage::_MakeSpeechMessage() {}
