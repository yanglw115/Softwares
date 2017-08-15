#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 * 代码参考《Qt5开发及实例》第4章 Qt5基本对话框
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
