#ifndef CBASICINFO_H
#define CBASICINFO_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

class CBasicInfo : public QWidget
{
    Q_OBJECT
public:
    explicit CBasicInfo(QWidget *parent = 0);
    ~CBasicInfo();

signals:

public slots:
    void slotChangeHeadImg();

public:
    QLabel *m_pLabelNetName;
    QLineEdit *m_pLineEditNetName;
    QLabel *m_pLabelRealName;
    QLineEdit *m_pLineEditRealName;
    QLabel *m_pLabelSex;
    QComboBox *m_pComboxSex;
    QLabel *m_pLabelDepartment;
    QTextEdit *m_pTextEditDep;
    QLabel *m_pLabelAge;
    QLineEdit *m_pLineEditAge;
    QLabel *m_pLabelNoteInfo;
    QGridLayout *m_pGridLayoutLeft;

    QLabel *m_pLabelHead;
    QLabel *m_pLabelHeadImg;
    QPushButton *m_pButtonUpdateHeadImg;
    QHBoxLayout *m_pHBoxHead;

    QLabel *m_pLabelPersonalInfo;
    QTextEdit *m_pTextEditPerinfo;
    QVBoxLayout *m_pVBoxRight;

    QGridLayout *m_pGridLayoutAll;
};

#endif // CBASICINFO_H
