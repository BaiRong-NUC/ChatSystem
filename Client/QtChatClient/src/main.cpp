#include <widget/main_widget/mainwidget.h>
#include <widget/friend_widget/choose_friend_widget/choose_friend_widget.h>
#include <widget/main_widget/rightwidget/message_widget.h>
#include <widget/main_widget/rightwidget/right_widget_title.h>
#include <widget/session_detail_widget/group_session_detai_widget.h>
#include <widget/session_detail_widget/single_session_detail_widget.h>
#include <QApplication>
#include <QDir>
#include <QDirIterator>
#include <QTimer>
#include <model/data.h>
#include <utils/log.h>

using namespace Log;

namespace
{
    void LoadApplicationStyle(QApplication &application)
    {
        QStringList styleFiles;
        QDirIterator styleIterator(":/styles", QStringList{"*.qss"}, QDir::Files, QDirIterator::Subdirectories);
        while (styleIterator.hasNext()) { styleFiles.append(styleIterator.next()); }
        styleFiles.sort(Qt::CaseSensitive);

        QString applicationStyle;
        for (const QString &stylePath : styleFiles)
        {
            QFile styleFile(stylePath);
            if (!styleFile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                LogInfo(LogLevel::ERROR, "样式文件加载失败: " + stylePath);
                continue;
            }
            applicationStyle.append(QString::fromUtf8(styleFile.readAll()));
            applicationStyle.append('\n');
        }
        application.setStyleSheet(applicationStyle);
    }
}  // namespace

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoadApplicationStyle(a);
    auto w = std::make_unique<ChatWidget::MainWidget>();
    w->show();

    // 为AddressSanitizer/CI提供可正常析构并退出的无交互冒烟模式。
    if (QCoreApplication::arguments().contains("--smoke-test"))
    {
        if (auto *addFriendButton = w->findChild<QPushButton *>("addFriendButton"))
        {
            addFriendButton->click();
        }
        if (w->findChild<ChatWidget::AddFriendWidget *>() == nullptr)
        {
            LogInfo(LogLevel::ERROR, "添加朋友窗口冒烟测试失败:点击按钮后未创建窗口");
            return 1;
        }

        // 模拟用户依次点击“更多”和会话详情中的“添加”，覆盖真实弹窗入口。
        auto *moreButton = w->findChild<QPushButton *>("moreButton");
        if (moreButton == nullptr)
        {
            LogInfo(LogLevel::ERROR, "选择好友窗口冒烟测试失败:未找到会话更多按钮");
            return 1;
        }
        auto *rightWidgetTitle = w->findChild<ChatWidget::RightWidgetTitle *>();
        if (rightWidgetTitle == nullptr)
        {
            LogInfo(LogLevel::ERROR, "会话详情冒烟测试失败:未找到右侧标题栏");
            return 1;
        }

        // 验证聊天消息区域使用自动隐藏的覆盖滚动条，而不是带箭头的原生滚动条。
        auto *messageWidget = w->findChild<ChatWidget::MessageWidget *>();
        auto *sessionScrollArea =
            w->findChild<ChatWidget::AutoHideScrollArea *>("midSessionAreaWidget");
        if (messageWidget == nullptr || sessionScrollArea == nullptr ||
            messageWidget->m_overlayScrollBar == nullptr ||
            sessionScrollArea->m_overlayScrollBar == nullptr ||
            !a.styleSheet().contains("autoHideScrollAreaOverlayScrollBar") ||
            messageWidget->verticalScrollBarPolicy() != Qt::ScrollBarAlwaysOff)
        {
            LogInfo(LogLevel::ERROR, "聊天消息区域冒烟测试失败:覆盖滚动条未正确初始化");
            return 1;
        }
        QEvent enterMessageWidgetEvent(QEvent::Enter);
        QApplication::sendEvent(messageWidget, &enterMessageWidgetEvent);
        if (messageWidget->verticalScrollBar()->maximum() > 0 &&
            messageWidget->m_overlayScrollBar->isHidden())
        {
            LogInfo(LogLevel::ERROR, "聊天消息区域冒烟测试失败:鼠标进入后覆盖滚动条未显示");
            return 1;
        }
        if (messageWidget->m_overlayScrollBar->geometry().right() !=
            messageWidget->rect().right())
        {
            LogInfo(LogLevel::ERROR, "聊天消息区域冒烟测试失败:覆盖滚动条未贴齐组件右侧");
            return 1;
        }
        QEvent leaveMessageWidgetEvent(QEvent::Leave);
        QApplication::sendEvent(messageWidget, &leaveMessageWidgetEvent);
        if (!messageWidget->m_overlayScrollBar->isHidden())
        {
            LogInfo(LogLevel::ERROR, "聊天消息区域冒烟测试失败:鼠标离开后覆盖滚动条未隐藏");
            return 1;
        }

        // 先验证单聊分支以及单聊中的添加群成员入口。
        rightWidgetTitle->isSingleSession = true;
        rightWidgetTitle->m_titleLabel->setText(QStringLiteral("好友1"));
        moreButton->click();

        auto *addGroupButton = w->findChild<QPushButton *>("addGroupButton");
        if (addGroupButton == nullptr)
        {
            LogInfo(LogLevel::ERROR, "选择好友窗口冒烟测试失败:未找到添加群聊按钮");
            return 1;
        }
        addGroupButton->click();

        auto *chooseFriendWidget = w->findChild<ChatWidget::ChooseFriendWidget *>();
        auto *sessionDetailWidget = w->findChild<ChatWidget::SingleSessionDetailWidget *>();
        if (chooseFriendWidget == nullptr ||
            sessionDetailWidget == nullptr ||
            chooseFriendWidget->findChild<ChatWidget::SearchBox *>() == nullptr ||
            chooseFriendWidget->m_totalFriendScrollArea == nullptr ||
            chooseFriendWidget->m_selectedFriendScrollArea == nullptr ||
            chooseFriendWidget->m_selectedFriendNames != QStringList{QStringLiteral("好友1")})
        {
            LogInfo(LogLevel::ERROR, "选择好友窗口冒烟测试失败:弹窗、搜索框或默认成员状态不正确");
            return 1;
        }

        // 人为留下搜索词和额外勾选项，再次点击“添加”后必须得到一个全新的默认状态窗口。
        if (chooseFriendWidget->m_friendItems.isEmpty())
        {
            LogInfo(LogLevel::ERROR, "选择好友窗口冒烟测试失败:缺少调试联系人");
            return 1;
        }
        chooseFriendWidget->m_friendItems.first()->SetSelected(true);
        chooseFriendWidget->m_searchBox->SetKeyword(QStringLiteral("安然"));
        chooseFriendWidget->reject();

        const QPointer<ChatWidget::ChooseFriendWidget> previousChooseFriendWidget = chooseFriendWidget;
        addGroupButton->click();
        chooseFriendWidget = sessionDetailWidget->m_chooseFriendWidget;
        if (chooseFriendWidget == nullptr || chooseFriendWidget == previousChooseFriendWidget ||
            chooseFriendWidget->m_selectedFriendNames != QStringList{QStringLiteral("好友1")} ||
            chooseFriendWidget->m_searchBox == nullptr ||
            chooseFriendWidget->m_searchBox->m_searchEdit == nullptr ||
            !chooseFriendWidget->m_searchBox->m_searchEdit->text().isEmpty() ||
            chooseFriendWidget->m_friendItems.isEmpty() ||
            chooseFriendWidget->m_friendItems.first()->m_isSelected)
        {
            LogInfo(LogLevel::ERROR, "选择好友窗口冒烟测试失败:再次打开后没有重置选择状态");
            return 1;
        }

        // 再切换到群聊分支，验证群成员、群信息、设置项和四个暂不实现功能的占位控件。
        chooseFriendWidget->reject();
        rightWidgetTitle->isSingleSession = false;
        rightWidgetTitle->m_titleLabel->setText(QStringLiteral("文博十三号宿舍楼群"));
        moreButton->click();
        auto *groupSessionDetailWidget = rightWidgetTitle->m_groupSessionDetailWidget.data();
        if (groupSessionDetailWidget == nullptr || !groupSessionDetailWidget->isVisible() ||
            groupSessionDetailWidget->m_memberSearchBox == nullptr ||
            groupSessionDetailWidget->m_memberItems.isEmpty() ||
            groupSessionDetailWidget->m_groupAnnouncementLabel == nullptr ||
            groupSessionDetailWidget->m_foldChatSwitch == nullptr ||
            groupSessionDetailWidget->m_followMembersButton == nullptr ||
            groupSessionDetailWidget->m_saveToContactsSwitch == nullptr ||
            groupSessionDetailWidget->m_scrollArea->m_overlayScrollBar == nullptr ||
            groupSessionDetailWidget->m_scrollArea->verticalScrollBarPolicy() != Qt::ScrollBarAlwaysOff)
        {
            LogInfo(LogLevel::ERROR, "群聊详情窗口冒烟测试失败:会话类型分流或界面控件不完整");
            return 1;
        }

        // 模拟指针进入和离开群详情窗口，验证覆盖滚动条会显示并再次隐藏。
        QEvent enterGroupDetailEvent(QEvent::Enter);
        QApplication::sendEvent(groupSessionDetailWidget->m_scrollArea, &enterGroupDetailEvent);
        if (groupSessionDetailWidget->m_scrollArea->verticalScrollBar()->maximum() > 0 &&
            groupSessionDetailWidget->m_scrollArea->m_overlayScrollBar->isHidden())
        {
            LogInfo(LogLevel::ERROR, "群聊详情窗口冒烟测试失败:鼠标进入后覆盖滚动条未显示");
            return 1;
        }
        if (groupSessionDetailWidget->m_scrollArea->m_overlayScrollBar->geometry().right() !=
            groupSessionDetailWidget->m_scrollArea->rect().right())
        {
            LogInfo(LogLevel::ERROR, "群聊详情窗口冒烟测试失败:覆盖滚动条未贴齐组件右侧");
            return 1;
        }
        QEvent leaveGroupDetailEvent(QEvent::Leave);
        QApplication::sendEvent(groupSessionDetailWidget->m_scrollArea, &leaveGroupDetailEvent);
        if (!groupSessionDetailWidget->m_scrollArea->m_overlayScrollBar->isHidden())
        {
            LogInfo(LogLevel::ERROR, "群聊详情窗口冒烟测试失败:鼠标离开后覆盖滚动条未隐藏");
            return 1;
        }
        QTimer::singleShot(100, &a, &QCoreApplication::quit);
    }

    return QApplication::exec();
}
