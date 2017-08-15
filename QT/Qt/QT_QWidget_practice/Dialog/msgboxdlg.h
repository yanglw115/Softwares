#ifndef MSGBOXDLG_H
#define MSGBOXDLG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLayout>

class MsgBoxDlg : public QDialog
{
    Q_OBJECT

public:
    MsgBoxDlg(QWidget *parent = 0);
    ~MsgBoxDlg();

private slots:
    void slotShowQuestMsg();
    void slotShowInfoMsg();
    void slotShowWarnMsg();
    void slotShowCritMsg();
    void slotShowAboutMsg();
    void slotShowQtAboutMsg();

private:
    QLabel *m_pLabel;
    QPushButton *m_pButtonQuest;
    QPushButton *m_pButtonInfo;
    QPushButton *m_pButtonWarn;
    QPushButton *m_pButtonCrit;
    QPushButton *m_pButtonAbout;
    QPushButton *m_pButtonQtAbout;

    QGridLayout *m_pGridLayout;

};

#endif // MSGBOXDLG_H
