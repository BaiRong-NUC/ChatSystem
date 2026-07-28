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

    constexpr auto kMessageTextEditStyleSheet =
        "QTextEdit { color: #eeeeee; background-color: #191919; border: none; font-size: 14px;"
        " selection-background-color: #3b7f5b; padding: 0px; }";

    constexpr const char *kToolButtonIconPaths[] = {
        ":/images/image.png",
        ":/images/file.png",
        ":/images/sound.png",
        ":/images/history.png",
    };

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

MessageEdit *MessageEdit::s_instance = nullptr;  // 初始化单例实例指针

MessageEdit *MessageEdit::GetInstance(QWidget *parent)
{
    if (s_instance == nullptr) { s_instance = new MessageEdit(parent); }
    return s_instance;
}

MessageEdit::MessageEdit(QWidget *parent) : QWidget(parent)
{
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
    for (const char *iconPath : kToolButtonIconPaths)
    {
        QPushButton *toolButton = new QPushButton(this);
        toolButton->setObjectName("toolButton");
        toolButton->setFixedSize(34, 30);
        toolButton->setIcon(MakeTintedIcon(iconPath, QColor("#c8c8c8"), QSize(20, 20)));
        toolButton->setIconSize(QSize(20, 20));
        toolButton->setCursor(Qt::PointingHandCursor);
        toolLayout->addWidget(toolButton);
    }
    toolLayout->addStretch();
    mainLayout->addLayout(toolLayout);

    this->m_textEdit = new QTextEdit(this);
    this->m_textEdit->setPlaceholderText("请输入消息内容...");
    this->m_textEdit->setAcceptRichText(false);
    this->m_textEdit->setStyleSheet(kMessageTextEditStyleSheet);
    mainLayout->addWidget(this->m_textEdit);

    QHBoxLayout *sendLayout = new QHBoxLayout();
    sendLayout->setContentsMargins(0, 0, 0, 0);
    sendLayout->addStretch();
    this->m_sendButton = new QPushButton("发送(S)", this);
    this->m_sendButton->setObjectName("sendButton");
    this->m_sendButton->setFixedSize(92, 34);
    this->m_sendButton->setCursor(Qt::PointingHandCursor);
    sendLayout->addWidget(this->m_sendButton);
    mainLayout->addLayout(sendLayout);
}

MessageEdit::~MessageEdit() = default;
