#include <widget/component/search_box.h>

using namespace ChatWidget;

namespace
{
    constexpr int kSearchBoxHeight = 46;
    constexpr int kSearchIconSize = 21;

    /**
     * 搜索图标使用 QPainter 绘制，避免公共组件依赖某张固定颜色、固定分辨率的位图。
     * 图标控件带有父对象，由 SearchBox 的 Qt 对象树负责销毁。
     */
    class SearchIconWidget final : public QWidget
    {
       public:
        explicit SearchIconWidget(QWidget *parent = nullptr) : QWidget(parent)
        {
            this->setFixedSize(kSearchIconSize, kSearchIconSize);
            this->setAttribute(Qt::WA_TransparentForMouseEvents);
        }

       protected:
        void paintEvent(QPaintEvent *event) override
        {
            Q_UNUSED(event);

            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(QPen(QColor("#a9aaad"), 2.1, Qt::SolidLine, Qt::RoundCap));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(QRectF(2.5, 2.5, 12, 12));
            painter.drawLine(QPointF(13.5, 13.5), QPointF(19, 19));
        }
    };
}  // namespace

SearchBox::SearchBox(QWidget *parent) : QWidget(parent)
{
    this->m_searchEdit = new QLineEdit(this);
    this->_InitSearchBox();
}

void SearchBox::_InitSearchBox()
{
    if (this->m_searchEdit == nullptr) { return; }

    this->setObjectName("searchBox");
    this->setProperty("focused", false);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setFixedHeight(kSearchBoxHeight);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout *searchLayout = new QHBoxLayout(this);
    searchLayout->setContentsMargins(15, 0, 10, 0);
    searchLayout->setSpacing(10);

    // 图标创建时已经指定父对象；加入布局只负责几何管理，不产生第二份所有权。
    SearchIconWidget *searchIcon = new SearchIconWidget(this);
    searchLayout->addWidget(searchIcon, 0, Qt::AlignVCenter);

    this->m_searchEdit->setObjectName("searchBoxEdit");
    this->m_searchEdit->setPlaceholderText(QStringLiteral("搜索"));
    this->m_searchEdit->setClearButtonEnabled(true);
    this->m_searchEdit->setFrame(false);
    this->m_searchEdit->setAccessibleName(QStringLiteral("搜索"));
    this->m_searchEdit->installEventFilter(this);
    searchLayout->addWidget(this->m_searchEdit, 1);

    connect(this->m_searchEdit, &QLineEdit::textChanged, this,
            [this](const QString &keyword) { emit this->KeywordChanged(keyword.trimmed()); });
    connect(this->m_searchEdit, &QLineEdit::returnPressed, this,
            [this]() { emit this->SearchRequested(this->GetKeyword()); });
}

void SearchBox::_RefreshFocusStyle(bool focused)
{
    this->setProperty("focused", focused);
    // QSS 动态属性改变后需要重新 polish，才能立即刷新搜索框外层边框。
    this->style()->unpolish(this);
    this->style()->polish(this);
    this->update();
}

bool SearchBox::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->m_searchEdit)
    {
        if (event->type() == QEvent::FocusIn) { this->_RefreshFocusStyle(true); }
        if (event->type() == QEvent::FocusOut) { this->_RefreshFocusStyle(false); }
    }
    return QWidget::eventFilter(watched, event);
}

void SearchBox::SetPlaceholderText(const QString &placeholderText)
{
    if (this->m_searchEdit == nullptr) { return; }
    this->m_searchEdit->setPlaceholderText(placeholderText);
    this->m_searchEdit->setAccessibleName(placeholderText);
}

void SearchBox::SetKeyword(const QString &keyword)
{
    if (this->m_searchEdit != nullptr) { this->m_searchEdit->setText(keyword); }
}

QString SearchBox::GetKeyword() const
{
    return this->m_searchEdit == nullptr ? QString() : this->m_searchEdit->text().trimmed();
}

void SearchBox::Clear()
{
    if (this->m_searchEdit != nullptr) { this->m_searchEdit->clear(); }
}

void SearchBox::SetSearchFocus()
{
    if (this->m_searchEdit != nullptr) { this->m_searchEdit->setFocus(Qt::OtherFocusReason); }
}
