#include <QApplication>
#include <QGraphicsScene>

#include "YLW_main_window.h"
#include "YLW_butter_fly.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setSceneRect(-200, -200, 400, 400);

    ButterFly *pButterFly = new ButterFly;
    pButterFly->setPos(-100, 0);

    scene->addItem(pButterFly);

    QGraphicsView *view = new QGraphicsView;
    view->setScene(scene);
    view->resize(400, 400);
    view->show();

    return a.exec();
}
