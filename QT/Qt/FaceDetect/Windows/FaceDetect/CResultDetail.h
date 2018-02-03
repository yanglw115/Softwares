#ifndef CRESULTDETAIL_H
#define CRESULTDETAIL_H

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPixmap>

#include "glob.h"

class CResultDetail : public QWidget
{
    Q_OBJECT
public:
    explicit CResultDetail(enumItemType type, QWidget *parent = 0);
    virtual ~CResultDetail();
    void setImagePath(const QString &strImagePath);
    void setData(const QString &strData);
    void setData(const QString &strData1, const QString &strData2);
    void setData(const QString &strData1, const QString &strData2, const QString &strData3);
    void setData(const QString &strData1, const QString &strData2, const QString &strData3,
                 const QString &strData4, const QString &strData5);
signals:

public slots:
private:
    enumItemType m_type;
    QString m_strImagePath;
    QLabel *m_pLabelImage;
    QLabel *m_pLabelItem;
    QPixmap m_pixmap;
    QLineEdit *m_pEditTop;
    QLineEdit *m_pEditMiddle1;
    QLineEdit *m_pEditMiddle2;
    QLineEdit *m_pEditMiddle3;
    QLineEdit *m_pEditBottom;

    QVBoxLayout *m_pVLayoutMain;
    QHBoxLayout *m_pHLayoutData;
    QGridLayout *m_pGridData;
};

#endif // CRESULTDETAIL_H
