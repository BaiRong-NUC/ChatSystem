#include <widget/rightwidget/message_edit.h>

using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr auto kMessageEditPanelStyleSheet =
        "QWidget#messageEditPanel { background-color: #191919; border-top: 1px solid #303030; }"
        "QPushButton#toolButton { background-color: transparent; border: none; border-radius: 4px; }"
        "QPushButton#toolButton:hover { background-color: #2b2b2b; }"
        "QPushButton#toolButton:pressed { background-color: #343434; }"
        "QPushButton#sendButton { color: #777777; background-color: #292929; border: none;"
        " border-radius: 5px; font-size: 13px; padding: 0 18px; }"
        "QPushButton#sendButton:hover { color: #d8d8d8; background-color: #333333; }";

    constexpr auto kPlainMessageTextEditStyleSheet =
        "QPlainTextEdit { color: #eeeeee; background-color: #191919; border: none; font-size: 14px;"
        " selection-background-color: #3b7f5b; padding: 0px; }";

    constexpr auto kSendImageButtonIconPath = ":/images/image.png";
    constexpr auto kSendFileButtonIconPath = ":/images/file.png";
    constexpr auto kSendSpeechButtonIconPath = ":/images/sound.png";
    constexpr auto kHistoryButtonIconPath = ":/images/history.png";

    QIcon MakeTintedIcon(const QString &path, const QColor &color, const QSize &size)
    {
        QPixmap pixmap(path);
        pixmap = pixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPainter painter(&pixmap);
        painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        painter.fillRect(pixmap.rect(), color);
        painter.end();
        return QIcon(pixmap);
    }
}  // namespace

MessageEdit::MessageEdit(QWidget *parent) : QWidget(parent)
{
    // 初始化资源
    this->m_textEdit = new QPlainTextEdit(this);
    this->m_sendButton = new QPushButton("发送(S)", this);
    this->m_sendImageButton = new QPushButton(this);
    this->m_sendFileButton = new QPushButton(this);
    this->m_sendSpeechButton = new QPushButton(this);
    this->m_historyButton = new QPushButton(this);

    // 初始化UI
    this->_InitMessageEdit();
}

void MessageEdit::_InitMessageEdit()
{
    this->setObjectName("messageEditPanel");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setFixedHeight(180);
    this->setStyleSheet(kMessageEditPanelStyleSheet);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(18, 10, 18, 12);
    mainLayout->setSpacing(4);

    QHBoxLayout *toolLayout = new QHBoxLayout();
    toolLayout->setContentsMargins(0, 0, 0, 0);
    toolLayout->setSpacing(6);

    auto initToolButton = [toolLayout](QPushButton *button, const char *iconPath)
    {
        button->setObjectName("toolButton");
        button->setFixedSize(34, 30);
        button->setIcon(MakeTintedIcon(iconPath, QColor("#c8c8c8"), QSize(20, 20)));
        button->setIconSize(QSize(20, 20));
        button->setCursor(Qt::PointingHandCursor);
        toolLayout->addWidget(button);
    };

    initToolButton(this->m_sendImageButton, kSendImageButtonIconPath);
    initToolButton(this->m_sendFileButton, kSendFileButtonIconPath);
    initToolButton(this->m_sendSpeechButton, kSendSpeechButtonIconPath);
    initToolButton(this->m_historyButton, kHistoryButtonIconPath);

    toolLayout->addStretch();
    mainLayout->addLayout(toolLayout);

    this->m_textEdit->setPlaceholderText("请输入消息内容...");
    this->m_textEdit->setStyleSheet(kPlainMessageTextEditStyleSheet);
    mainLayout->addWidget(this->m_textEdit);

    QHBoxLayout *sendLayout = new QHBoxLayout();
    sendLayout->setContentsMargins(0, 0, 0, 0);
    sendLayout->addStretch();
    this->m_sendButton->setObjectName("sendButton");
    this->m_sendButton->setFixedSize(92, 34);
    this->m_sendButton->setCursor(Qt::PointingHandCursor);
    sendLayout->addWidget(this->m_sendButton);
    mainLayout->addLayout(sendLayout);
}

MessageEdit::~MessageEdit() = default;
