#include "mainDialog.h"
#include <QApplication>
#include <QtDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Dialog w;
    w.show();
    return a.exec();
}
