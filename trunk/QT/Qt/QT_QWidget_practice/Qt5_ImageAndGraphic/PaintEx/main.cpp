#include "YLW_main_widget.h"
#include <QApplication>

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 * 参考《Qt5开发及实例》6.2 Qt基础图形的绘制
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    w.show();

    return a.exec();
}
