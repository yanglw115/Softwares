#ifndef CDETAILINFO_H
#define CDETAILINFO_H

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QGridLayout>
#include <QTextEdit>
#include <QWidget>

typedef QMap<QString, QStringList> MapProvince_t;

class CDetailInfo : public QWidget
{
    Q_OBJECT
public:
    explicit CDetailInfo(QWidget *parent = 0);
    ~CDetailInfo();

signals:

public slots:
    void slotChangProvinceStringList(int index);

private:
    QStringList m_strListCountry;
    MapProvince_t m_mapProvince;
    QLabel *m_pLabelCountry;
    QComboBox *m_pComboBoxCountry;
    QLabel *m_pLabelProvince;
    QComboBox *m_pComboBoxProvince;
    QLabel *m_pLabelCity;
    QLineEdit *m_pLineEditCity;
    QLabel *m_pLabelPersonal;
    QTextEdit *m_pTextEditPersonal;

    QGridLayout *m_pGridLayout;
};

#endif // CDETAILINFO_H
