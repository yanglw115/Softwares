#ifndef CCONNECTIIONS_H
#define CCONNECTIIONS_H

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

class CConnectiions : public QWidget
{
    Q_OBJECT
public:
    explicit CConnectiions(QWidget *parent = 0);
    ~CConnectiions();

signals:

public slots:
private:
    QLabel *m_pLabelEmail;
    QLineEdit *m_pLineEditEmail;
    QLabel *m_pLabelAddress;
    QLineEdit *m_pLineEditAddress;
    QLabel *m_pLabelPostNumber;
    QLineEdit *m_pLineEditPostNumber;
    QLabel *m_pLabelPhoneNumber;
    QLineEdit *m_pLineEditPhoneNumber;
    QCheckBox *m_pCheckGetSaveMessage;
    QLabel *m_pLabelFixNumber;
    QLineEdit *m_pLineEditFixNumber;

    QGridLayout *m_pGridLayout;
};

#endif // CCONNECTIIONS_H
