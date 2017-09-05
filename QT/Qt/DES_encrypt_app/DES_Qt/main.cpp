#include "YLW_main_widget.h"
#include <QApplication>
#include <QTextCodec>
#include <QTextStream>
#include <QFile>
#include <QtDebug>

#include "YLW_VS_char_set.h"
#include "norwegianwoodstyle.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QFile fileQss(":/darkStyleQss/darkStyle.qss");
    if (fileQss.open(QFile::ReadOnly)) {
        QTextStream textFile(&fileQss);
        QString strStyleSheet = textFile.readAll();
        a.setStyleSheet(strStyleSheet);
    } else {
        qDebug() << "Open QSS file failed!";
    }

    //QApplication::setStyle(new NorwegianWoodStyle);

    MainWidget w;
    w.show();

    return a.exec();
}
