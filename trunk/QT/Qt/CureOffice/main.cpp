#include "mainwindow.h"
#include <QApplication>

#include <QtXlsx/QtXlsx>
#include "cure_log.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CMyLog::initLog();

    MainWindow w;
    w.show();

    return a.exec();
}
