#include "mainwindow.h"
#include "mydrawer.h"
#include <QApplication>

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 * 参考自《Qt5开发及实例.pdf》3.5
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    MyDrawer drawer;
    drawer.show();

    return a.exec();
}
