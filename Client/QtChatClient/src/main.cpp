#include <widget/mainwidget.h>
#include <QApplication>
#include <QTimer>
#include <model/data.h>
#include <utils/log.h>

using namespace Log;

namespace
{
    void LoadApplicationStyle(QApplication &application)
    {
        const QStringList styleFiles = {
            ":/styles/common.qss",
            ":/styles/navigation.qss",
            ":/styles/session_list.qss",
            ":/styles/chat_view.qss",
            ":/styles/message_edit.qss",
        };

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
    auto w = std::make_unique<MainWidget>();
    w->show();

    // 为AddressSanitizer/CI提供可正常析构并退出的无交互冒烟模式。
    if (QCoreApplication::arguments().contains("--smoke-test"))
    {
        QTimer::singleShot(100, &a, &QCoreApplication::quit);
    }

    return QApplication::exec();
}
