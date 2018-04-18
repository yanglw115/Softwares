#include "mainwindow.h"
#include <QApplication>

#include <QtXlsx/QtXlsx>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if 1
    MainWindow w;
    w.show();
#else
    QXlsx::Document xlsx;
    xlsx.write("A1", "Hello Qt!");
    xlsx.saveAs("Test.xlsx");
    return 0;
#endif
    return a.exec();
}
