#include "mainwindow.h"
#include <QApplication>
#include "CDetectResult.h"
#include "CResultDetail.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    CDetectResult result;
    result.show();

    return a.exec();
}
