#include "mainwidget.h"

#include <QApplication>
#include <model/data.h>
#include <utils/log.h>

using namespace Log;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // SetLogLevel(LogLevel::INFO);  // 设置日志级别为INFO,输出INFO及以上级别的日志
    SetLogLevel(LogLevel::WARNING);  // 设置日志级别为WARNING,输出WARNING及以上级别的日志
    LogInfo(LogLevel::INFO, "应用程序启动");
    LogInfo(LogLevel::WARNING, "这是一个警告日志");
    LogInfo(LogLevel::ERROR, "这是一个错误日志");
    MainWidget w;
    w.show();
    return QApplication::exec();
}
