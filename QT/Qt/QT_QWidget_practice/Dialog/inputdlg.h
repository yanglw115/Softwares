#ifndef INPUTDLG_H
#define INPUTDLG_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>


class InputDlg : public QDialog
{
    Q_OBJECT
public:
    InputDlg(QWidget *parent = 0);
    ~InputDlg();
private slots:
    void slotChangeName();
    void slotChangeAge();
    void slotChangeSex();
    void slotChangeScore();

private:
    QLabel *m_pLabelName;
    QLabel *m_pLabelNameV;
    QLabel *m_pLabelAge;
    QLabel *m_pLabelAgeV;
    QLabel *m_pLabelSex;
    QLabel *m_pLabelSexV;
    QLabel *m_pLabelScore;
    QLabel *m_pLabelScoreV;

    QPushButton *m_pButtonName;
    QPushButton *m_pButtonAge;
    QPushButton *m_pButtonSex;
    QPushButton *m_pButtonScore;

    QGridLayout *m_pGridLayout;

};

#endif // INPUTDLG_H
