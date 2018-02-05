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
#include "CObjectResult.h"

class CFaceDetecter: public QWidget
{
public:
    CFaceDetecter(QWidget * parent = 0, Qt::WindowFlags f = 0);
    virtual ~CFaceDetecter();
    void startDetect(const QString &strImgPath, const enumItemType type);
    CObjectResult& getObjResultRef() { return *m_pObjResult; }

private:
    void initWindow();
private:
    CObjectResult *m_pObjResult;
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
    CResultDetail *m_pDetailPimples;
    CResultDetail *m_pDetailBlackheads;
    CResultDetail *m_pDetailFaceColor;
    CResultDetail *m_pDetailPore;
    CResultDetail *m_pDetailCoarse;
};

#endif // CFACEDETECTER_H