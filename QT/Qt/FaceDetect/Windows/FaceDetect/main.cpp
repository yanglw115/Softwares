#include "mainwindow.h"
#include <QApplication>
#include "CFaceDetecter.h"
#include "CResultDetail.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //CFaceDetecter result;
    //result.show();

    return a.exec();
}
