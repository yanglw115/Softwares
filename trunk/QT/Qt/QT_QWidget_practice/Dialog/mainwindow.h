#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QBoxLayout>

#include "inputdlg.h"
#include "msgboxdlg.h"
#include "progressdlg.h"
#include "pallete.h"

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotSetFileName();
    void slotSetColor();
    void slotSetFont();
    void slotOpenInputDlg();
    void slotSetInputPointer();
    void slotShowMsgBoxDlg();
    void slotShowProgressBar();
    void slotShowPalette();

private:
    QPushButton *m_pButtonFileOpen;
    QLineEdit *m_pLineFileOpen;

    QPushButton *m_pButtonColor;
    QFrame *m_pFrameColor;

    QPushButton *m_pButtonFont;
    QLineEdit *m_pLineFontOpen;

    QPushButton *m_pButtonInput;
    InputDlg *m_pInputDlg;

    QPushButton *m_pButtonMsgBox;
    MsgBoxDlg *m_pMsgBoxDlg;

    QPushButton *m_pButtonProgress;
    ProgressDlg *m_pProgressDlg;

    QPushButton *m_pButtonPalette;
    Pallete *m_pPaletteDilg;

    QGridLayout *m_pGridLaout;

};

#endif // MAINWINDOW_H
