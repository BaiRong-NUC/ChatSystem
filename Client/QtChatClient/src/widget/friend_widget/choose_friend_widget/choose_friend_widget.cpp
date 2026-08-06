#include <widget/friend_widget/choose_friend_widget/choose_friend_widget.h>

using namespace ChatWidget;

namespace
{
    constexpr int kWindowWidth = 920;
    constexpr int kWindowHeight = 640;
    constexpr int kLeftPanelWidth = 410;
    constexpr int kFooterHeight = 92;
    constexpr int kActionButtonWidth = 150;
    constexpr int kActionButtonHeight = 44;

    AutoHideScrollArea *CreateScrollArea(const QString &objectName, QWidget *parent,
                                         QWidget **contentWidget)
    {
        AutoHideScrollArea *scrollArea = new AutoHideScrollArea(parent);
        scrollArea->setObjectName(objectName);
        scrollArea->setWidgetResizable(true);
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        QWidget *content = new QWidget(scrollArea);
        content->setObjectName(objectName + "Content");
        content->setAttribute(Qt::WA_StyledBackground, true);

        QVBoxLayout *contentLayout = new QVBoxLayout(content);
        contentLayout->setContentsMargins(0, 0, 0, 0);
        contentLayout->setSpacing(0);
        contentLayout->setAlignment(Qt::AlignTop);

        // setWidget 会把 content 的 QObject 所有权交给 QScrollArea。
        scrollArea->setWidget(content);
        *contentWidget = content;
        return scrollArea;
    }
}  // namespace

ChooseFriendWidget::ChooseFriendWidget(QWidget *parent) : QDialog(parent)
{
    this->m_searchBox = new SearchBox(this);
    this->m_selectedCountLabel = new QLabel(this);
    this->m_confirmButton = new QPushButton(this);
    this->m_cancelButton = new QPushButton(this);

    this->_InitChooseFriendWidget();
    this->_InitSignalSlots();
}

void ChooseFriendWidget::_InitChooseFriendWidget()
{
    if (this->m_searchBox == nullptr || this->m_selectedCountLabel == nullptr ||
        this->m_confirmButton == nullptr || this->m_cancelButton == nullptr)
    {
        return;
    }

    this->setObjectName("chooseFriendWidget");
    this->setWindowTitle(QStringLiteral("发起群聊"));
    this->setWindowIcon(QIcon(":/images/logo.png"));
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setModal(true);
    this->setFixedSize(kWindowWidth, kWindowHeight);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(1, 1, 1, 1);
    mainLayout->setSpacing(0);

    // 左侧：公共搜索框、联系人标题以及可滚动的全部好友列表。
    QWidget *leftPanel = new QWidget(this);
    leftPanel->setObjectName("chooseFriendLeftPanel");
    leftPanel->setFixedWidth(kLeftPanelWidth);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(32, 28, 0, 20);
    leftLayout->setSpacing(0);

    this->m_searchBox->SetPlaceholderText(QStringLiteral("搜索"));
    leftLayout->addWidget(this->m_searchBox);
    leftLayout->addSpacing(28);

    QLabel *contactTitle = new QLabel(QStringLiteral("联系人"), leftPanel);
    contactTitle->setObjectName("chooseFriendContactTitle");
    contactTitle->setFixedHeight(36);
    leftLayout->addWidget(contactTitle);
    leftLayout->addSpacing(8);

    QWidget *totalFriendContent = nullptr;
    this->m_totalFriendScrollArea =
        CreateScrollArea("chooseFriendTotalScrollArea", leftPanel, &totalFriendContent);
    this->m_totalFriendListWidget = totalFriendContent;
    leftLayout->addWidget(this->m_totalFriendScrollArea, 1);

    // 中间分隔线属于窗口装饰，由父对象自动释放。
    QFrame *divider = new QFrame(this);
    divider->setObjectName("chooseFriendDivider");
    divider->setFrameShape(QFrame::VLine);
    divider->setFixedWidth(1);

    // 右侧：标题、已选择好友列表和固定在底部的操作按钮。
    QWidget *rightPanel = new QWidget(this);
    rightPanel->setObjectName("chooseFriendRightPanel");
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(38, 28, 32, 0);
    rightLayout->setSpacing(0);

    QWidget *titleRow = new QWidget(rightPanel);
    titleRow->setObjectName("chooseFriendTitleRow");
    titleRow->setFixedHeight(46);
    QHBoxLayout *titleLayout = new QHBoxLayout(titleRow);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(16);

    QLabel *titleLabel = new QLabel(QStringLiteral("发起群聊"), titleRow);
    titleLabel->setObjectName("chooseFriendTitle");
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();

    this->m_selectedCountLabel->setObjectName("chooseFriendSelectedCount");
    this->m_selectedCountLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    titleLayout->addWidget(this->m_selectedCountLabel);
    rightLayout->addWidget(titleRow);
    rightLayout->addSpacing(14);

    QWidget *selectedFriendContent = nullptr;
    this->m_selectedFriendScrollArea =
        CreateScrollArea("chooseFriendSelectedScrollArea", rightPanel, &selectedFriendContent);
    this->m_selectedFriendListWidget = selectedFriendContent;
    rightLayout->addWidget(this->m_selectedFriendScrollArea, 1);

    QWidget *footer = new QWidget(rightPanel);
    footer->setObjectName("chooseFriendFooter");
    footer->setFixedHeight(kFooterHeight);
    QHBoxLayout *footerLayout = new QHBoxLayout(footer);
    footerLayout->setContentsMargins(0, 20, 0, 18);
    footerLayout->setSpacing(78);

    this->m_confirmButton->setObjectName("chooseFriendConfirmButton");
    this->m_confirmButton->setText(QStringLiteral("完成"));
    this->m_confirmButton->setAccessibleName(QStringLiteral("完成好友选择"));
    this->m_confirmButton->setCursor(Qt::PointingHandCursor);
    this->m_confirmButton->setFixedSize(kActionButtonWidth, kActionButtonHeight);

    this->m_cancelButton->setObjectName("chooseFriendCancelButton");
    this->m_cancelButton->setText(QStringLiteral("取消"));
    this->m_cancelButton->setAccessibleName(QStringLiteral("取消好友选择"));
    this->m_cancelButton->setCursor(Qt::PointingHandCursor);
    this->m_cancelButton->setFixedSize(kActionButtonWidth, kActionButtonHeight);

    footerLayout->addStretch();
    footerLayout->addWidget(this->m_confirmButton);
    footerLayout->addWidget(this->m_cancelButton);
    footerLayout->addStretch();
    rightLayout->addWidget(footer);

    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(divider);
    mainLayout->addWidget(rightPanel, 1);

    this->_UpdateSelectedState();
    this->m_searchBox->SetSearchFocus();
}

void ChooseFriendWidget::_InitSignalSlots()
{
    if (this->m_searchBox == nullptr || this->m_confirmButton == nullptr || this->m_cancelButton == nullptr)
    {
        return;
    }

    connect(this->m_searchBox, &SearchBox::KeywordChanged, this, &ChooseFriendWidget::_FilterFriends);
    connect(this->m_searchBox, &SearchBox::SearchRequested, this, &ChooseFriendWidget::_FilterFriends);
    connect(this->m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(this->m_confirmButton, &QPushButton::clicked, this,
            [this]()
            {
                emit this->ConfirmSelectedFriends(this->m_selectedFriendNames);
                this->accept();
            });
}

void ChooseFriendWidget::_FilterFriends(const QString &keyword)
{
    const QString normalizedKeyword = keyword.trimmed();
    for (const QPointer<ChooseFriendItem> &friendItem : this->m_friendItems)
    {
        if (friendItem == nullptr) { continue; }
        const bool matched = normalizedKeyword.isEmpty() ||
                             friendItem->m_name.contains(normalizedKeyword, Qt::CaseInsensitive);
        friendItem->setVisible(matched);
    }
}

void ChooseFriendWidget::_AddSelectedFriendItem(const QIcon &icon, const QString &name,
                                                 ChooseFriendItem *sourceItem)
{
    if (this->m_selectedFriendListWidget == nullptr) { return; }

    // 同一个左侧好友只能在右侧出现一次；预选好友没有 sourceItem，允许由调用方逐个追加。
    if (sourceItem != nullptr)
    {
        for (const SelectedFriendRelation &relation : this->m_selectedRelations)
        {
            if (relation.m_sourceItem == sourceItem && relation.m_selectedItem != nullptr) { return; }
        }
    }

    QVBoxLayout *selectedLayout = qobject_cast<QVBoxLayout *>(this->m_selectedFriendListWidget->layout());
    if (selectedLayout == nullptr) { return; }

    ChooseFriendItem *selectedItem = new ChooseFriendItem(this->m_selectedFriendListWidget);
    selectedItem->AddFriend(icon, name, true);
    selectedItem->SetRemoveMode(true);
    selectedLayout->addWidget(selectedItem);

    SelectedFriendRelation relation;
    relation.m_sourceItem = sourceItem;
    relation.m_selectedItem = selectedItem;
    this->m_selectedRelations.append(relation);

    const QPointer<ChooseFriendItem> guardedSourceItem = sourceItem;
    const QPointer<ChooseFriendItem> guardedSelectedItem = selectedItem;
    connect(selectedItem, &ChooseFriendItem::SelectionChanged, this,
            [this, guardedSourceItem, guardedSelectedItem](bool isSelected)
            {
                if (isSelected) { return; }

                if (guardedSourceItem != nullptr)
                {
                    // SetSelected 会触发左侧条目的 SelectionChanged，并由统一路径移除右侧条目。
                    guardedSourceItem->SetSelected(false);
                    return;
                }
                this->_RemoveSelectedFriendItem(nullptr, guardedSelectedItem);
                this->_UpdateSelectedState();
            });
}

void ChooseFriendWidget::_RemoveSelectedFriendItem(ChooseFriendItem *sourceItem,
                                                    ChooseFriendItem *selectedItem)
{
    for (qsizetype index = this->m_selectedRelations.size() - 1; index >= 0; --index)
    {
        const SelectedFriendRelation &relation = this->m_selectedRelations.at(index);
        const bool sourceMatched = sourceItem != nullptr && relation.m_sourceItem == sourceItem;
        const bool selectedMatched = selectedItem != nullptr && relation.m_selectedItem == selectedItem;
        if (!sourceMatched && !selectedMatched) { continue; }

        QPointer<ChooseFriendItem> itemToRemove = relation.m_selectedItem;
        this->m_selectedRelations.removeAt(index);
        if (itemToRemove != nullptr)
        {
            // 当前函数可能由 itemToRemove 自身的信号进入，deleteLater 可避免在信号栈中销毁发送者。
            itemToRemove->hide();
            itemToRemove->deleteLater();
        }
        break;
    }
}

void ChooseFriendWidget::_UpdateSelectedState()
{
    // 清理因父对象先销毁而自动置空的关系，避免数量标签包含失效条目。
    for (qsizetype index = this->m_selectedRelations.size() - 1; index >= 0; --index)
    {
        if (this->m_selectedRelations.at(index).m_selectedItem == nullptr)
        {
            this->m_selectedRelations.removeAt(index);
        }
    }

    this->m_selectedFriendNames.clear();
    for (const SelectedFriendRelation &relation : this->m_selectedRelations)
    {
        if (relation.m_selectedItem != nullptr)
        {
            this->m_selectedFriendNames.append(relation.m_selectedItem->m_name);
        }
    }
    if (this->m_selectedCountLabel != nullptr)
    {
        this->m_selectedCountLabel->setText(
            QStringLiteral("已选择%1个联系人").arg(this->m_selectedFriendNames.size()));
    }
    if (this->m_confirmButton != nullptr)
    {
        this->m_confirmButton->setEnabled(!this->m_selectedFriendNames.isEmpty());
    }

    emit this->SelectionChanged(this->m_selectedFriendNames);
}

void ChooseFriendWidget::AddFriend(const QIcon &icon, const QString &name, bool isSelected)
{
    if (this->m_totalFriendListWidget == nullptr) { return; }

    QVBoxLayout *totalFriendLayout = qobject_cast<QVBoxLayout *>(this->m_totalFriendListWidget->layout());
    if (totalFriendLayout == nullptr) { return; }

    ChooseFriendItem *friendItem = new ChooseFriendItem(this->m_totalFriendListWidget);
    friendItem->AddFriend(icon, name, false);
    totalFriendLayout->addWidget(friendItem);
    this->m_friendItems.append(friendItem);

    const QPointer<ChooseFriendItem> guardedFriendItem = friendItem;
    connect(friendItem, &ChooseFriendItem::SelectionChanged, this,
            [this, guardedFriendItem](bool selected)
            {
                if (guardedFriendItem == nullptr) { return; }
                if (selected)
                {
                    this->_AddSelectedFriendItem(guardedFriendItem->m_icon,
                                                 guardedFriendItem->m_name, guardedFriendItem);
                }
                else
                {
                    this->_RemoveSelectedFriendItem(guardedFriendItem);
                }
                this->_UpdateSelectedState();
            });

    // 连接建立后再恢复初始状态，确保左右列表和数量标签同步更新。
    if (isSelected) { friendItem->SetSelected(true); }
}

void ChooseFriendWidget::AddSelectedFriend(const QIcon &icon, const QString &name)
{
    this->_AddSelectedFriendItem(icon, name);
    this->_UpdateSelectedState();
}
