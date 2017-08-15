#ifndef BUTTERFLY_H
#define BUTTERFLY_H

#include <QObject>
#include <QRectF>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

class ButterFly : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit ButterFly(QObject *parent = 0);
    ~ButterFly();
    QRectF boundingRect() const; /* virtual function */
    void timerEvent(QTimerEvent *event);

protected:
    /* virtual function */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    bool m_bUp;
    QPixmap m_pixmapUp;
    QPixmap m_pixmapDown;
    qreal m_angle;


public slots:
};

#endif // BUTTERFLY_H
