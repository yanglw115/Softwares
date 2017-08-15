#include "mywindow.h"
#include <QApplication>

#include "cdigiclock.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //myWindow w;
    //w.show();

    CDigiClock clock;
    clock.show();

    return a.exec();
}
