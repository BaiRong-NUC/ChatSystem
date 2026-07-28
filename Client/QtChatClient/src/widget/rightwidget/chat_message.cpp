#include <widget/rightwidget/chat_message.h>

using namespace ChatWidget;
using namespace Model;
using namespace Log;

namespace
{
    constexpr auto kTextLabelStyle = "QLabel { color: #2c3e50; line-height: 1.5; }";
}

ChatMessage::ChatMessage(Model::Message *message, QWidget *parent) : QWidget(parent), m_message(message)
{
    // 资源初始化
    this->m_message = message;
    // 初始化UI
    this->_InitChatMessage();
}
ChatMessage::~ChatMessage()
{
    if (this->m_message != nullptr) { delete this->m_message; }
    this->m_message = nullptr;
}

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
    QObject *parent = this->parent();
    // 1. 获取父元素大小
    if (!parent->isWidgetType())
    {
        LogInfo(LogLevel::ERROR, "TextChatMessage父元素不是QWidget类型");
        return;
    }
    QWidget *parentWidget = static_cast<QWidget *>(parent);
    int width = parentWidget->width() * (CHAT_MESSAGE_WIDTH);

    // 2. 计算行/高
    // 计算如果放一行放置需要多宽
    QFontMetrics fontMetrics(*this->m_textFont);
    int textWidth = fontMetrics.horizontalAdvance(this->m_textLabel->text());
    // 计算行数
    int lineCount = (textWidth / (width - 40)) + 1;  // 左右空20px边距
    if (lineCount == 1)
    {
        width = textWidth + 40;  // 左右空20px边距
    }
    // 根据行数计算高度,每行高度为字体高度+行间距
    int height = lineCount * (this->m_textLabel->font().pixelSize()) * 1.5 + 20;

    // 3. 设置控件大小
    QPainter painter(this);
    QPainterPath path;
    // 设置抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);
    // 绘制圆角矩形背景
    if (this->m_isLeft)
    {
        // 左侧消息,圆角矩形背景颜色为白色
        painter.setBrush(QColor(255, 255, 255));
        painter.setPen(QColor(255, 255, 255));

        // 相对于父位置
        painter.drawRoundedRect(10, 0, width, height, 10, 10);

        // 画三角形
        path.moveTo(10, 15);
        path.lineTo(0, 20);
        path.lineTo(10, 25);
        path.closeSubpath();
        painter.drawPath(path);

        this->m_textLabel->setGeometry(20, 10, width, height);
    }
    else
    {
        // 右侧消息,圆角矩形背景颜色为绿色
        painter.setBrush(QColor(0, 255, 0));
        painter.setPen(QColor(0, 255, 0));

        // 左侧边坐标
        int leftPos = this->width() - width - 10;
        // 右侧边坐标
        int rightPos = this->width() - 10;
        painter.drawRoundedRect(leftPos, 0, width, height, 10, 10);

        // 画三角形
        path.moveTo(rightPos - 10, 15);
        path.lineTo(rightPos, 20);
        path.lineTo(rightPos - 10, 25);
        path.closeSubpath();
        painter.drawPath(path);

        this->m_textLabel->setGeometry(leftPos + 10, 10, width - 20, height - 20);
    }

    // 4. 设置控件大小
    parentWidget->setFixedHeight(height + 50);  // 设置父控件高度,加10px间距,确保文字可以完全显示
}

TextChatMessage::TextChatMessage(QString &text, bool isLeft, QFont *textFont, QWidget *parent)
    : QWidget(parent), m_isLeft(isLeft), m_textFont(textFont)
{
    // 资源初始化
    this->m_textLabel = new QLabel(this);
    this->m_textLabel->setText(text);
    this->m_textLabel->setFont(*this->m_textFont);
    this->m_textLabel->setWordWrap(true);  // 设置文本自动换行
    this->m_textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    this->m_textLabel->setStyleSheet(kTextLabelStyle);
}

TextChatMessage::~TextChatMessage()
{
    if (this->m_textFont != nullptr) { delete this->m_textFont; }
    if (this->m_textLabel != nullptr) { delete this->m_textLabel; }
    this->m_textFont = nullptr;
    this->m_textLabel = nullptr;
}

void ChatMessage::_MakeTextMessage() {}

void ChatMessage::_MakeImageMessage() {}

void ChatMessage::_MakeFileMessage() {}

void ChatMessage::_MakeSpeechMessage() {}
