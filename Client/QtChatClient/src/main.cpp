#include <widget/mainwidget.h>
#include <QApplication>
#include <QTimer>
#include <model/data.h>
#include <utils/log.h>

using namespace Log;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto w = std::make_unique<MainWidget>();
    w->show();

    // 为AddressSanitizer/CI提供可正常析构并退出的无交互冒烟模式。
    if (QCoreApplication::arguments().contains("--smoke-test"))
    {
        QTimer::singleShot(100, &a, &QCoreApplication::quit);
    }

    return QApplication::exec();
}
