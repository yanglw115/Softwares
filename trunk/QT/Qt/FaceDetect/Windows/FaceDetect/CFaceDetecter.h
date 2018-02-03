#ifndef CFACEDETECTER_H
#define CFACEDETECTER_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QPixmap>
#include <QStackedWidget>

#include "vs_charset.h"
#include "CResultDetail.h"

class CFaceDetecter: public QWidget
{
public:
    CFaceDetecter(QWidget * parent = 0, Qt::WindowFlags f = 0);
    virtual ~CFaceDetecter();
    void startDetect(const QString &strImgPath, const enumItemType type);

private:
    void initWindow();
private:
    QHBoxLayout *m_pHLayoutMain;
    QVBoxLayout *m_pVLayoutLeft;
    QVBoxLayout *m_pVLayoutRight;
    QLabel *m_pLabelResult;
    QLabel *m_pLabelDetails;
    QFrame *m_pLineLeft;
    QFrame *m_pLineRight;
    QFrame *m_pLineMiddle;
    QListWidget *m_pListWidget;
    QStackedWidget *m_pStacked;
};

#endif // CFACEDETECTER_H
