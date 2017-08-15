#include "YLW_main_window.h"
#include <QApplication>
#include <QFontDatabase>
#include <QtDebug>

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 * 参考自<<Qt5开发及实例>> 6.3 Qt5双缓冲机制
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFontDatabase fontData;
    foreach (QString str, fontData.families()) {
        qDebug() << str;
    }

    QFont font;
    font.setFamily("Microsoft YaHei UI");
    a.setFont(font);

    MainWindow w;
    w.show();

    return a.exec();
}
