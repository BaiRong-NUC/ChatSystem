#include <widget/midwidget/midwidget.h>
using namespace ChatWidget;
using namespace Log;

MidWidget *MidWidget::s_instance = nullptr;  // 初始化单例实例指针

MidWidget *MidWidget::GetInstance(QWidget *parent)
{
    if (s_instance == nullptr) { s_instance = new MidWidget(parent); }
    return s_instance;
}

MidWidget::MidWidget(QWidget *parent) : QWidget(parent)
{
    // 初始化窗口资源
    // 按钮
    this->m_addFriendButton = new QPushButton(this);
    // 搜索框
    this->m_searchLineEdit = new QLineEdit(this);
    // Icon
    this->m_addFriendButtonIconPath = ":/images/cross.png";

    // 初始化UI界面
    this->_InitMidWidget();

    // 初始化信号槽,在UI资源初始化完毕后
    this->_InitSignalSlots();
}

MidWidget::~MidWidget() = default;

void MidWidget::_InitMidWidget()
{
    // 窗口名称,大小和背景
    this->setObjectName("midWidget");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setFixedWidth(200);
    this->setStyleSheet("QWidget#midWidget { background-color: #949ea9; }");

    // 组件样式

    // 搜索框
    this->m_searchLineEdit->setPlaceholderText("搜索");
    this->m_searchLineEdit->setFixedHeight(30);
    this->m_searchLineEdit->setStyleSheet(
        "QLineEdit { background-color: #aeb4ba; border: none; border-radius: 5px; }"
        "QLineEdit:focus { background-color: #ccd1d8; border: none; border-radius: 5px; }");

    // 添加好友按钮
    this->m_addFriendButton->setFixedSize(30, 30);
    this->m_addFriendButton->setIcon(QIcon(this->m_addFriendButtonIconPath));
    this->m_addFriendButton->setStyleSheet(
        "QPushButton { background-color: #aeb4ba; border: none;border-radius: 5px; }"
        "QPushButton:hover { background-color: #ccd1d8; border: none;border-radius: 5px; }");
    // 布局
    QGridLayout *midLayout = new QGridLayout(this);
    midLayout->setContentsMargins(0, 17, 0, 0);
    midLayout->setHorizontalSpacing(0);

    // 占位
    QWidget *leftPlaceholder = new QWidget(this);
    leftPlaceholder->setFixedWidth(13);
    midLayout->addWidget(leftPlaceholder, 0, 0);
    midLayout->addWidget(this->m_searchLineEdit, 0, 1);  // 搜索框

    QWidget *buttonPlaceholder = new QWidget(this);
    buttonPlaceholder->setFixedWidth(6);
    midLayout->addWidget(buttonPlaceholder, 0, 2);
    midLayout->addWidget(this->m_addFriendButton, 0, 3);  // 添加好友按钮

    QWidget *rightPlaceholder = new QWidget(this);
    rightPlaceholder->setFixedWidth(13);
    midLayout->addWidget(rightPlaceholder, 0, 4);

    QWidget *topPlaceholder = new QWidget(this);
    topPlaceholder->setFixedHeight(5);
    midLayout->addWidget(topPlaceholder, 1, 0, 1, 5);  // 占位,横跨一行,占满所有列

    // 好友信息框
    MidSessionAreaWidget *sessionFriendArea = new MidSessionAreaWidget(this);
    midLayout->addWidget(sessionFriendArea, 2, 0, 1, 5);  // 好友信息框

#if DEBUG_CODE
    LogInfo(LogLevel::INFO, "MidWidget::_InitMidWidget() 添加测试项");
    sessionFriendArea->SelectSessionItem(0);
#endif
}

void MidWidget::_InitSignalSlots() {}