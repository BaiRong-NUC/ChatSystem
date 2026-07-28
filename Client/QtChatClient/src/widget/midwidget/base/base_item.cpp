#include <widget/midwidget/base/base_item.h>
#include <QStyle>

using namespace ChatWidget;
using namespace Log;

namespace
{
    void RefreshStyle(QWidget *widget)
    {
        widget->style()->unpolish(widget);
        widget->style()->polish(widget);
        widget->update();
    }
}  // namespace

BaseItem::BaseItem(QWidget *owner, QWidget *parent, const QIcon &icon, const QString &name, const QString &text)
    : QWidget(parent)
{
    // 初始化资源
    this->m_midSessionAreaWidget = owner;  // (item->parent(m_midWidgetListWidget)->parent(MidSessionWidgetArea))
    this->m_isSelected = false;            // 默认未选中
    this->m_iconButton = new QPushButton(this);
    this->m_nameLabel = new QLabel(this);
    this->m_textLabel = new QLabel(this);

    // 初始化窗口UI资源
    this->_InitBaseItem(icon, name, text);
}

BaseItem::~BaseItem() = default;

void BaseItem::_InitBaseItem(const QIcon &icon, const QString &name, const QString &text)
{
    if (this->m_iconButton == nullptr || this->m_nameLabel == nullptr || this->m_textLabel == nullptr)
    {
        LogInfo(LogLevel::ERROR, "base item 资源初始化失败");
        exit(-1);
    }

    this->setFixedHeight(76);
    // 设置背景
    this->setObjectName("baseItem");
    this->setProperty("selected", false);
    this->setAttribute(Qt::WA_StyledBackground, true);

    // 布局
    QGridLayout *baseItemLayout = new QGridLayout(this);
    baseItemLayout->setContentsMargins(14, 8, 12, 8);
    baseItemLayout->setHorizontalSpacing(12);
    baseItemLayout->setVerticalSpacing(0);
    this->setLayout(baseItemLayout);

    // base项头像按钮
    this->m_iconButton->setFixedSize(52, 52);
    this->m_iconButton->setIconSize(QSize(52, 52));
    this->m_iconButton->setIcon(icon);
    this->m_iconButton->setObjectName("baseItemIconButton");
    // base项名称标签
    this->m_nameLabel->setText(name);
    this->m_nameLabel->setFixedHeight(28);
    this->m_nameLabel->setObjectName("baseItemNameLabel");
    // base项文本标签
    this->m_textLabel->setText(text);
    this->m_textLabel->setFixedHeight(26);
    this->m_textLabel->setObjectName("baseItemTextLabel");

    this->m_iconButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  // 设置按钮大小策略为固定
    this->m_nameLabel->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Fixed);  // 设置标签大小策略为水平扩展，垂直固定
    this->m_textLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 添加组件到布局
    baseItemLayout->addWidget(this->m_iconButton, 0, 0, 2, 2);  // 占两行两列
    baseItemLayout->addWidget(this->m_nameLabel, 0, 2, 1, 1);   // 第一行
    baseItemLayout->addWidget(this->m_textLabel, 1, 2, 1, 1);   // 第二行
}

// 鼠标点击事件处理函数
void BaseItem::HandleLeftClick() {}
void BaseItem::HandleRightClick() {}
void BaseItem::mousePressEvent(QMouseEvent *event)
{
    this->m_isSelected = true;
    this->setProperty("selected", true);
    RefreshStyle(this);
    // 更新其他好友项的选中状态为未选中
    if (this->m_midSessionAreaWidget != nullptr)
    {
        QList<BaseItem *> friendItems = this->m_midSessionAreaWidget->findChildren<BaseItem *>();
        for (BaseItem *item : friendItems)
        {
            if (item != this)
            {
                item->m_isSelected = false;
                item->setProperty("selected", false);
                RefreshStyle(item);
            }
        }
    }

    // 左键点击选中好友项,右键点击显示菜单
    if (event->button() == Qt::LeftButton)
    {
        // LogInfo(LogLevel::INFO, "left clicked: name = " + this->m_nameLabel->text());
        this->HandleLeftClick();
    }
    else if (event->button() == Qt::RightButton)
    {
        // LogInfo(LogLevel::INFO, "right clicked: name = " + this->m_nameLabel->text());
        this->HandleRightClick();
    }
}
