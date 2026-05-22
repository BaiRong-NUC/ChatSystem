#include "mainwidget.h"

#include <QApplication>
#include <model/data.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << Model::Message::_GenerateUniqueMessageId();  // 测试生成唯一消息ID
    MainWidget w;
    w.show();
    return QApplication::exec();
}
