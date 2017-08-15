#include "YLW_geometry.h"
#include <QApplication>


/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 * <<QT5开发与实例>> 第6章 Qt5图形与图片
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Geometry w;
    w.show();

    return a.exec();
}
