#include <widget/main_widget/midwidget/mid_widget_title.h>

using namespace ChatWidget;
using namespace Log;

namespace
{
    constexpr auto kSearchLineEditPlaceholderText = "搜索";  // 搜索框占位符文本

    class SearchIconWidget : public QWidget
    {
       public:
        explicit SearchIconWidget(QWidget *parent) : QWidget(parent)
        {
            this->setFixedSize(20, 20);
            this->setAttribute(Qt::WA_TransparentForMouseEvents);
        }

       protected:
        void paintEvent(QPaintEvent *event) override
        {
            Q_UNUSED(event);
            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(QPen(QColor("#b2b2b2"), 2));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(QRectF(2, 2, 11, 11));
            painter.drawLine(QPointF(12, 12), QPointF(18, 18));
        }
    };

    QIcon MakePlusIcon()
    {
        QPixmap pixmap(20, 20);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(QColor("#b2b2b2"), 1.5));
        painter.drawLine(QPointF(2, 10), QPointF(18, 10));
        painter.drawLine(QPointF(10, 2), QPointF(10, 18));
        return QIcon(pixmap);
    }
}  // namespace

MidWidgetTitle::MidWidgetTitle(QWidget *parent) : QWidget(parent)
{
    // 初始化资源
    this->m_addFriendButton = new QPushButton(this);
    this->m_searchLineEdit = new QLineEdit(this);

    this->_InitMidWidgetTitle();
    this->_InitSignalSlots();
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
    this->setFixedHeight(74);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 搜索框
    this->m_searchLineEdit->setObjectName("sessionSearchEdit");
    this->m_searchLineEdit->setPlaceholderText(kSearchLineEditPlaceholderText);
    this->m_searchLineEdit->setFixedHeight(38);
    SearchIconWidget *searchIcon = new SearchIconWidget(this->m_searchLineEdit);
    searchIcon->move(10, 9);
    searchIcon->show();

    // 添加好友按钮
    this->m_addFriendButton->setObjectName("addFriendButton");
    this->m_addFriendButton->setFixedSize(38, 38);
    this->m_addFriendButton->setIcon(MakePlusIcon());
    this->m_addFriendButton->setIconSize(QSize(20, 20));
    this->m_addFriendButton->setAccessibleName(QStringLiteral("添加朋友"));
    this->m_addFriendButton->setCursor(Qt::PointingHandCursor);

    // 布局
    QGridLayout *midWidgetLayout = new QGridLayout(this);
    midWidgetLayout->setContentsMargins(14, 18, 14, 18);
    midWidgetLayout->setHorizontalSpacing(0);

    midWidgetLayout->addWidget(this->m_searchLineEdit, 0, 0);  // 搜索框

    QWidget *buttonPlaceholder = new QWidget(this);
    buttonPlaceholder->setFixedWidth(10);
    midWidgetLayout->addWidget(buttonPlaceholder, 0, 1);
    midWidgetLayout->addWidget(this->m_addFriendButton, 0, 2);  // 添加好友按钮
}

void MidWidgetTitle::_InitSignalSlots()
{
    connect(this->m_addFriendButton, &QPushButton::clicked, this, &MidWidgetTitle::addFriendRequested);
}
