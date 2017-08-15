#include "YLW_butter_fly.h"

const qreal g_PI = 3.1415926;

ButterFly::ButterFly(QObject *parent)
    : QObject(parent)
    , m_bUp(true)
{
    m_pixmapUp.load(":img/butterflyUp.png");
    m_pixmapDown.load(":img/butterflyDown.png");
    startTimer(100);
}

ButterFly::~ButterFly()
{

}

QRectF ButterFly::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-m_pixmapUp.width() / 2 - adjust, -m_pixmapUp.height() / 2 - adjust,
                  m_pixmapUp.width() + adjust * 2, m_pixmapUp.height() + adjust * 2);
}

void ButterFly::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(this->boundingRect().topLeft(), m_bUp? m_pixmapUp: m_pixmapDown);
    m_bUp = !m_bUp;
}

void ButterFly::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    qreal edgeX = this->scene()->sceneRect().right() + this->boundingRect().width() / 2;
    qreal edgeTop = this->scene()->sceneRect().top() + this->boundingRect().height() / 2;
    qreal edgeBottom = this->scene()->sceneRect().bottom() + this->boundingRect().height() / 2;

    if (this->pos().x() >= edgeX) {
        this->setPos(this->scene()->sceneRect().left(), this->pos().y());
    }
    if (this->pos().y() <= edgeTop) {
        this->setPos(this->pos().x(), this->scene()->sceneRect().bottom());
    }
    if (this->pos().y() >= edgeBottom) {
        this->setPos(this->pos().x(), this->scene()->sceneRect().top());
    }

    m_angle += (qrand() % 10) / 20.0; /* 0~0.5 */
    qreal dx = fabs(sin(m_angle * g_PI) * 10); /* 0~10 */
    qreal dy = qrand() % 20 - 10.0; /* -10~10 */
    this->setPos(mapToParent(dx, dy));
}
