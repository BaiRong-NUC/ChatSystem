#include <widget/component/add_group.h>

#include <QStyleOptionButton>

using namespace ChatWidget;

namespace
{
    constexpr int kAddGroupWidth = 92;
    constexpr int kAddGroupHeight = 100;
    constexpr int kAddButtonSize = 56;
    constexpr int kNameWidth = 76;
    constexpr int kNameHeight = 24;
    constexpr int kPlusHalfLength = 13;

    class AddGroupButton final : public QPushButton
    {
       public:
        explicit AddGroupButton(QWidget *parent = nullptr) : QPushButton(parent) {}

       protected:
        void paintEvent(QPaintEvent *event) override
        {
            QPushButton::paintEvent(event);

            QStyleOptionButton option;
            this->initStyleOption(&option);

            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(QPen(option.palette.color(QPalette::ButtonText), 2, Qt::SolidLine, Qt::RoundCap));

            const QPoint center(this->rect().center());
            painter.drawLine(center.x() - kPlusHalfLength, center.y(), center.x() + kPlusHalfLength, center.y());
            painter.drawLine(center.x(), center.y() - kPlusHalfLength, center.x(), center.y() + kPlusHalfLength);
        }
    };
}  // namespace

AddGroup::AddGroup(QWidget *parent) : QWidget(parent)
{
    this->m_addButton = new AddGroupButton(this);
    this->m_nameLabel = new QLabel(QStringLiteral("添加"), this);
    this->_InitAddGroup();
}

void AddGroup::_InitAddGroup()
{
    this->setObjectName("addGroup");
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setFixedSize(kAddGroupWidth, kAddGroupHeight);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 7, 8, 7);
    layout->setSpacing(6);
    layout->addWidget(this->m_addButton, 0, Qt::AlignHCenter);
    layout->addWidget(this->m_nameLabel, 0, Qt::AlignHCenter);

    this->m_addButton->setObjectName("addGroupButton");
    this->m_addButton->setFixedSize(kAddButtonSize, kAddButtonSize);
    this->m_addButton->setCursor(Qt::PointingHandCursor);
    this->m_addButton->setFocusPolicy(Qt::NoFocus);
    this->m_addButton->setAccessibleName(QStringLiteral("创建群聊"));
    this->m_addButton->setToolTip(QStringLiteral("创建群聊"));

    this->m_nameLabel->setObjectName("addGroupNameLabel");
    this->m_nameLabel->setAlignment(Qt::AlignCenter);
    this->m_nameLabel->setFixedSize(kNameWidth, kNameHeight);
}
