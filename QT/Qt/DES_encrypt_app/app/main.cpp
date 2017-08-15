#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "des_source.h"
#include "YLW_database_model.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<CMyDES>("qt.DES.component", 1, 0, "MyDES");
    qmlRegisterType<DatabaseModel>("qt.DES.component", 1, 0, "DatabaseModel");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
