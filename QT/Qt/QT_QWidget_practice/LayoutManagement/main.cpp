#include "mainwindow.h"
#include <QApplication>

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 * 例子来自《Qt5开发及实例》第3章
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
