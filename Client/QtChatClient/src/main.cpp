#include <widget/main_widget/mainwidget.h>
#include <widget/friend_widget/choose_friend_widget/choose_friend_widget.h>
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
        moreButton->click();

        auto *addGroupButton = w->findChild<QPushButton *>("addGroupButton");
        if (addGroupButton == nullptr)
        {
            LogInfo(LogLevel::ERROR, "选择好友窗口冒烟测试失败:未找到添加群聊按钮");
            return 1;
        }
        addGroupButton->click();

        auto *chooseFriendWidget = w->findChild<ChatWidget::ChooseFriendWidget *>();
        if (chooseFriendWidget == nullptr ||
            chooseFriendWidget->findChild<ChatWidget::SearchBox *>() == nullptr ||
            chooseFriendWidget->GetSelectedFriendNames() != QStringList{QStringLiteral("好友1")})
        {
            LogInfo(LogLevel::ERROR, "选择好友窗口冒烟测试失败:弹窗、搜索框或默认成员状态不正确");
            return 1;
        }
        QTimer::singleShot(100, &a, &QCoreApplication::quit);
    }

    return QApplication::exec();
}
