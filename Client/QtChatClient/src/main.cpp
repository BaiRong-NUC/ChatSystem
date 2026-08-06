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

        // 覆盖选择好友窗口的真实构造、条目选择和公共搜索框创建路径。
        // 窗口带有 MainWidget 父对象，程序退出时由 Qt 对象树统一释放。
        auto *chooseFriendWidget = new ChatWidget::ChooseFriendWidget(w.get());
        chooseFriendWidget->AddFriend(QIcon(":/images/defaultAvatar.png"), QStringLiteral("测试好友一"));
        chooseFriendWidget->AddFriend(QIcon(":/images/defaultAvatar.png"), QStringLiteral("测试好友二"), true);
        chooseFriendWidget->show();
        if (chooseFriendWidget->findChild<ChatWidget::SearchBox *>() == nullptr ||
            chooseFriendWidget->GetSelectedFriendNames() != QStringList{QStringLiteral("测试好友二")})
        {
            LogInfo(LogLevel::ERROR, "选择好友窗口冒烟测试失败:搜索框或预选好友状态不正确");
            return 1;
        }
        QTimer::singleShot(100, &a, &QCoreApplication::quit);
    }

    return QApplication::exec();
}
