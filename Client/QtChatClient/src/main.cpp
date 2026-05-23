#include "mainwidget.h"

#include <QApplication>
#include <model/data.h>
#include <utils/log.h>

using namespace Log;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget *w = MainWidget::GetInstance();
    w->show();
    return QApplication::exec();
}
