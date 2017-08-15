#ifndef CDIGICLOCK_H
#define CDIGICLOCK_H

#include <QLCDNumber>
#include <QObject>

class QTimer;
class CDigiClock : public QLCDNumber
{
    Q_OBJECT

public:
    CDigiClock(QWidget *parent = 0);
    virtual ~CDigiClock();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void slotShowTime();

private:
    QTimer *m_pTimer;
    QPoint m_dragPoint;
    bool m_bShowColon;
};

#endif // CDIGICLOCK_H
