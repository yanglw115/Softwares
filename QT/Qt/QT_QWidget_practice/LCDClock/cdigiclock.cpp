
#include <QMouseEvent>
#include <QtDebug>
#include <QTimer>
#include <QTime>

#include "cdigiclock.h"

CDigiClock::CDigiClock(QWidget *parent)
    : QLCDNumber(parent)
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::green);
    this->setPalette(palette);

    /* 没有边框的frame,用户不能够手动move或resize它 */
    this->setWindowFlags(Qt::FramelessWindowHint);
    /* 设置透明度，0是完全透明，1.0是完全不透明(opaque) */
    this->setWindowOpacity(0.5);

    /* 设置显示时间区域的大小，显示文本的大小随之改变 */
    this->resize(240, 100);

    /* 设置数码时间的数量，这里的数量或长度包括分隔符,默认长度是5，所以如果要按照hh:mm:ss展示时，默认是不够的 */
    this->setDigitCount(8);
    /* 调试结果表明，下面的设置为true时，优先显示单位大的时间；默认优先显示小的时间(在digiCount不够用的情况下) */
    this->setSmallDecimalPoint(false);

    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(500);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(slotShowTime()));
    slotShowTime();
    m_pTimer->start(500);

    m_bShowColon = true;
}

CDigiClock::~CDigiClock()
{

}

void CDigiClock::mousePressEvent(QMouseEvent *event)
{
    /* event->button()返回引起当前事件的button，如果只是鼠标移动，则返回Qt::NoButton */
    if (Qt::LeftButton == event->button()) {
        /* event->globalPos()返回的是相对整个桌面的坐标左上角位置 */
        /* this->frameGemetry()返回的是窗口几何坐标 */
        /* 这里求出的是相对位置 */
        m_dragPoint = event->globalPos() - this->frameGeometry().topLeft();
        event->accept();
    }
}

void CDigiClock::mouseMoveEvent(QMouseEvent *event)
{
    /* event->buttons()返回鼠标状态，由Qt::LeftButton | Qt::RightButton | Qt::MidButton组成 */
    if (Qt::LeftButton & event->buttons()) {
        QPoint p = event->globalPos() - m_dragPoint;
        this->move(p);
        event->accept();
    }
}

void CDigiClock::slotShowTime()
{
    QString strTime = QTime::currentTime().toString("hh:mm:ss");
    //qDebug() << strTime;
    if (m_bShowColon) {
        m_bShowColon = false;
        strTime[5] = ':';
    } else {
        m_bShowColon = true;
        strTime[5] = ' ';
    }
    this->display(strTime);
}

