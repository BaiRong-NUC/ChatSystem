#include <widget/component/auto_hide_scroll_area.h>

using namespace ChatWidget;

namespace
{
    constexpr int kOverlayScrollBarWidth = 10;
}  // namespace

AutoHideScrollArea::AutoHideScrollArea(QWidget *parent) : QScrollArea(parent)
{
    // 直接叠放在整个滚动组件上，避免viewport边框或内部边距在右侧留下空隙。
    // 指定this为父对象后，组件销毁时覆盖滚动条会由Qt对象树自动释放。
    this->m_overlayScrollBar = new QScrollBar(Qt::Vertical, this);
    this->_InitAutoHideScrollArea();
}

void AutoHideScrollArea::_InitAutoHideScrollArea()
{
    if (this->m_overlayScrollBar == nullptr) { return; }

    // 原生滚动条继续保存范围和值，但不参与绘制，避免平台原生箭头和轨道出现。
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->m_overlayScrollBar->setObjectName("autoHideScrollAreaOverlayScrollBar");
    this->m_overlayScrollBar->setFixedWidth(kOverlayScrollBarWidth);
    this->m_overlayScrollBar->hide();
    this->m_overlayScrollBar->raise();

    // 自身负责进入/离开事件；viewport负责布局完成后的尺寸变化事件。
    this->installEventFilter(this);
    this->viewport()->installEventFilter(this);

    // 滚轮改变真实滚动条，拖动覆盖滚动条时再反向写回真实滚动条。
    connect(this->verticalScrollBar(), &QScrollBar::rangeChanged, this,
            [this](int, int) { this->_SyncOverlayScrollBarFromSource(); });
    connect(this->verticalScrollBar(), &QScrollBar::valueChanged,
            this->m_overlayScrollBar, &QScrollBar::setValue);
    connect(this->m_overlayScrollBar, &QScrollBar::valueChanged,
            this->verticalScrollBar(), &QScrollBar::setValue);
}

void AutoHideScrollArea::_SetScrollBarVisible(bool visible)
{
    if (this->m_overlayScrollBar == nullptr) { return; }

    const bool hasScrollableContent = this->verticalScrollBar()->maximum() >
                                      this->verticalScrollBar()->minimum();
    this->m_overlayScrollBar->setVisible(visible && hasScrollableContent);
    if (visible && hasScrollableContent) { this->m_overlayScrollBar->raise(); }
}

void AutoHideScrollArea::_UpdateOverlayScrollBarGeometry()
{
    if (this->m_overlayScrollBar == nullptr) { return; }

    // 使用AutoHideScrollArea自身尺寸，让滚动条外沿与窗口右边缘严格重合。
    const int scrollBarX = qMax(0, this->width() - kOverlayScrollBarWidth);
    this->m_overlayScrollBar->setGeometry(scrollBarX, 0, kOverlayScrollBarWidth,
                                          this->height());
    this->m_overlayScrollBar->raise();
}

void AutoHideScrollArea::_SyncOverlayScrollBarFromSource()
{
    if (this->m_overlayScrollBar == nullptr) { return; }

    QScrollBar *sourceScrollBar = this->verticalScrollBar();
    // 同步时临时屏蔽覆盖滚动条信号，防止两个滚动条互相触发形成重复通知。
    const bool previousSignalState = this->m_overlayScrollBar->blockSignals(true);
    this->m_overlayScrollBar->setRange(sourceScrollBar->minimum(), sourceScrollBar->maximum());
    this->m_overlayScrollBar->setPageStep(sourceScrollBar->pageStep());
    this->m_overlayScrollBar->setSingleStep(sourceScrollBar->singleStep());
    this->m_overlayScrollBar->setValue(sourceScrollBar->value());
    this->m_overlayScrollBar->blockSignals(previousSignalState);

    this->_SetScrollBarVisible(this->underMouse());
}

bool AutoHideScrollArea::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this)
    {
        if (event->type() == QEvent::Enter) { this->_SetScrollBarVisible(true); }
        else if (event->type() == QEvent::Leave) { this->_SetScrollBarVisible(false); }
    }
    else if (watched == this->viewport() && event->type() == QEvent::Resize)
    {
        this->RefreshScrollBar();
    }

    return QScrollArea::eventFilter(watched, event);
}

void AutoHideScrollArea::resizeEvent(QResizeEvent *event)
{
    QScrollArea::resizeEvent(event);
    this->RefreshScrollBar();
}

void AutoHideScrollArea::showEvent(QShowEvent *event)
{
    QScrollArea::showEvent(event);
    this->RefreshScrollBar();
}

void AutoHideScrollArea::RefreshScrollBar()
{
    this->_UpdateOverlayScrollBarGeometry();
    this->_SyncOverlayScrollBarFromSource();
}
