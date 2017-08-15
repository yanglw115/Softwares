#ifndef PROGRESSDLG_H
#define PROGRESSDLG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QProgressBar>
#include <QProgressDialog>
#include <QBoxLayout>

class ProgressDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDlg(QWidget *parent = 0);
    virtual ~ProgressDlg();

private slots:
    void slotStartProgress();

private:
    QLabel *m_pLabelFileNum;
    QLineEdit *m_pEditFileNum;
    QLabel *m_pLabelType;
    QComboBox *m_pComboBox;
    QProgressBar *m_pProgressBar;
    QPushButton *m_pButtonStart;
    QProgressDialog *m_pProgressDlg;
    QGridLayout *m_pGridLayout;
};

#endif // PROGRESSDLG_H
