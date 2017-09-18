#include "YLW_main_widget.h"
#include <QApplication>
#include <QTextCodec>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QtDebug>

#include "YLW_VS_char_set.h"
#include "norwegianwoodstyle.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWidget w;

    //QString filePath = "../DES_Qt/qss/darkStyle/darkStyle.qss";
    //QFile fileQss(filePath);
    QFile fileQss(":/darkStyleQss/darkStyle.qss");
    QFileInfo fileInfo(fileQss);
    //w.registerFileWatcher(fileInfo.absoluteFilePath());
    w.slotFileChanged(fileInfo.absoluteFilePath());

    //QApplication::setStyle(new NorwegianWoodStyle);
    w.show();

    return a.exec();
}
