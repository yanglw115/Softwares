#ifndef CUREEMAILDIALOG_H
#define CUREEMAILDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QLayout>

class CureEmailDialog : public QDialog
{
    Q_OBJECT
public:
    enum enumControlType {
        TYPE_SEND = 0,
        TYPE_CANCEL, /* 取消 */
        TYPE_CLOSE,
        TYPE_CANCELED, /* 已取消 */
        TYPE_FINISH
    };

    CureEmailDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);
    virtual ~CureEmailDialog();

    void initDialog();
    void setHRName(const QString &strName);
    void setTotalValue(const uint nValue);
    void setCheckedValue(const uint nValue);
    void setSuccessValue(const uint nValue);
    void setFailedValue(const uint nValue);
    void setProgressValue(const uint nValue);
    void setProgressRange(const uint nStart, const uint nEnd);
    int getProgressRange() const;
signals:
    void sigStartSend();
    void sigCancelSend();
private:
    void setSendStaticsVisible(bool bShow);
private slots:
    void slotHandleSendButton();
    void slotHandleCancelButton();
private:
    QLabel *m_pLabelSalaryDate;
    QLabel *m_pLabelHR;
    QLabel *m_pLabelTotal;
    QLabel *m_pLabelChecked;
    QLabel *m_pLabelSuccess;
    QLabel *m_pLabelFailed;

    QLabel *m_pLabelValueDate;
    QLabel *m_pLabelValueHR;
    QLabel *m_pLabelValueTotal;
    QLabel *m_pLabelValueChecked;
    QLabel *m_pLabelValueSuccess;
    QLabel *m_pLabelValueFailed;

    QProgressBar *m_pProgressBar;
    QPushButton *m_pButtonStart;
    QPushButton *m_pButtonCancel;

    QVBoxLayout *m_pMainLayout;
    enumControlType m_nSendButtonState;
};

#endif // CUREEMAILDIALOG_H
