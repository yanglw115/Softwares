#ifndef CCONTENT_H
#define CCONTENT_H

#include <QFrame>
#include <QStackedWidget>
#include <QWidget>

#include "cbasicinfo.h"
#include "cconnectiions.h"
#include "cdetailinfo.h"

class CContent: public QFrame
{
    Q_OBJECT
public:
    CContent(QWidget *parent = 0);
    ~CContent();

public slots:
    void slotSetCurrentStackWidget(int index);

private:
    QStackedWidget *m_pStackWidget;
    QPushButton *m_pButtonModify;
    QPushButton *m_pButtonClose;

    QHBoxLayout *m_pHBoxButton;
    QVBoxLayout *m_pVBoxAll;

    CBasicInfo *m_pBaseInfo;
    CConnectiions *m_pConnection;
    CDetailInfo *m_pDetailInfo;
};

#endif // CCONTENT_H
