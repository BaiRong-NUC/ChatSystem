#include <widget/midwidget/mid_widget_title.h>

using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr auto kAddFriendButtonIconPath = ":/images/cross.png";  // 添加好友按钮图标路径
    constexpr auto kSearchLineEditPlaceholderText = "搜索";          // 搜索框占位符文本
    constexpr auto kSearchEditStyleSheet =
        "QLineEdit { background-color: #aeb4ba; border: none; border-radius: 5px; }"
        "QLineEdit:focus { background-color: #ccd1d8; border: none; border-radius: 5px; }";  // 搜索框样式表
    constexpr auto kAddFriendButtonStyleSheet =
        "QPushButton { background-color: #aeb4ba; border: none;border-radius: 5px; }"
        "QPushButton:hover { background-color: #ccd1d8; border: none;border-radius: 5px; }";  // 添加好友按钮样式表
}  // namespace

MidWidgetTitle *MidWidgetTitle::s_instance = nullptr;  // 初始化单例实例指针

MidWidgetTitle *MidWidgetTitle::GetInstance(QWidget *parent)
{
    if (s_instance == nullptr) { s_instance = new MidWidgetTitle(parent); }
    return s_instance;
}

MidWidgetTitle::MidWidgetTitle(QWidget *parent) : QWidget(parent)
{
    // 初始化资源
    this->m_addFriendButton = new QPushButton(this);
    this->m_searchLineEdit = new QLineEdit(this);
    this->m_addFriendButtonIconPath = kAddFriendButtonIconPath;

    this->_InitMidWidgetTitle();
}

MidWidgetTitle::~MidWidgetTitle() = default;

void MidWidgetTitle::_InitMidWidgetTitle()
{
    if (this->m_addFriendButton == nullptr || this->m_searchLineEdit == nullptr)
    {
        LogInfo(LogLevel::ERROR, "MidWidgetTitle标题栏资源初始化失败");
        exit(-1);
    }
    // 组件样式

    // 搜索框
    this->m_searchLineEdit->setPlaceholderText(kSearchLineEditPlaceholderText);
    this->m_searchLineEdit->setFixedHeight(30);
    this->m_searchLineEdit->setStyleSheet(kSearchEditStyleSheet);

    // 添加好友按钮
    this->m_addFriendButton->setFixedSize(30, 30);
    this->m_addFriendButton->setIcon(QIcon(this->m_addFriendButtonIconPath));
    this->m_addFriendButton->setStyleSheet(kAddFriendButtonStyleSheet);
    // 布局
    QGridLayout *midWidgetLayout = new QGridLayout(this);
    midWidgetLayout->setContentsMargins(0, 17, 0, 0);
    midWidgetLayout->setHorizontalSpacing(0);

    // 占位
    QWidget *leftPlaceholder = new QWidget(this);
    leftPlaceholder->setFixedWidth(13);
    midWidgetLayout->addWidget(leftPlaceholder, 0, 0);
    midWidgetLayout->addWidget(this->m_searchLineEdit, 0, 1);  // 搜索框

    QWidget *buttonPlaceholder = new QWidget(this);
    buttonPlaceholder->setFixedWidth(6);
    midWidgetLayout->addWidget(buttonPlaceholder, 0, 2);
    midWidgetLayout->addWidget(this->m_addFriendButton, 0, 3);  // 添加好友按钮

    QWidget *rightPlaceholder = new QWidget(this);
    rightPlaceholder->setFixedWidth(13);
    midWidgetLayout->addWidget(rightPlaceholder, 0, 4);

    QWidget *topPlaceholder = new QWidget(this);
    topPlaceholder->setFixedHeight(5);
    midWidgetLayout->addWidget(topPlaceholder, 1, 0, 1, 5);  // 占位,横跨一行,占满所有列
}