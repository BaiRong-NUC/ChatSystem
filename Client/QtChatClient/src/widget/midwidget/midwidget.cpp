#include <widget/midwidget/midwidget.h>
using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr auto kMidWidgetStyle = "QWidget#midWidget { background-color: #949ea9; }";
}  // namespace

MidWidget *MidWidget::s_instance = nullptr;  // 初始化单例实例指针

MidWidget *MidWidget::GetInstance(QWidget *parent)
{
    if (s_instance == nullptr) { s_instance = new MidWidget(parent); }
    return s_instance;
}

MidWidget::MidWidget(QWidget *parent) : QWidget(parent)
{
    // 初始化窗口资源
    this->m_titleBar = MidWidgetTitle::GetInstance(this);  // 获取标题栏单例实例
    this->m_sessionArea = new MidSessionAreaWidget(this);  // 创建会话

    // 初始化UI界面
    this->_InitMidWidget();

    // 初始化信号槽,在UI资源初始化完毕后
    this->_InitSignalSlots();
}

MidWidget::~MidWidget()
{
    if (this->m_sessionArea != nullptr) { delete this->m_sessionArea; }
    this->m_sessionArea = nullptr;
}

void MidWidget::_InitMidWidget()
{
    if (this->m_titleBar == nullptr || this->m_sessionArea == nullptr)
    {
        LogInfo(LogLevel::ERROR, "MidWidget中间会话区资源初始化失败");
        exit(-1);
    }

    // 窗口名称,大小和背景
    this->setObjectName("midWidget");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setFixedWidth(200);
    this->setStyleSheet(kMidWidgetStyle);

    // 布局
    QGridLayout *midLayout = new QGridLayout(this);
    midLayout->setContentsMargins(0, 0, 0, 0);
    midLayout->setSpacing(0);
    midLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    this->setLayout(midLayout);

    // 标题栏
    midLayout->addWidget(this->m_titleBar, 0, 0, 1, 5);  // 标题栏占据第一行的所有列
    // 好友信息框
    midLayout->addWidget(this->m_sessionArea, 2, 0, 1, 5);  // 好友信息框占据第三行的所有列

#if DEBUG_CODE
    LogInfo(LogLevel::INFO, "MidWidget::_InitMidWidget() 添加测试项");
    this->m_sessionArea->SelectSessionItem(0);
#endif
}

void MidWidget::_InitSignalSlots() {}