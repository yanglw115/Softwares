#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QString strQssFile = "../QssStyleSheetDebuger/qss/MyStyle/MyStyle.qss";
    //QString strQssFile = "../blue.qss";
    w.registerFileWatcher(strQssFile);
    w.slotWatchedFileChanged(strQssFile);

    w.show();
    return a.exec();
}
