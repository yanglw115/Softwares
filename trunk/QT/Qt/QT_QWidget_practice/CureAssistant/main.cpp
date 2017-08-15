
#include <QApplication>

#include "main_widget.h"
#include "myLog.h"
#include "vs_char_set.h"

int main(int argc, char *argv[])
{
    qInstallMessageHandler(CMyLog::messageHandler);
    QApplication a(argc, argv);
    MainWidget *w = new MainWidget;
    w->show();

    return a.exec();
}
