#include <widget/main_widget/mainwidget.h>
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
        QTimer::singleShot(100, &a, &QCoreApplication::quit);
    }

    return QApplication::exec();
}
