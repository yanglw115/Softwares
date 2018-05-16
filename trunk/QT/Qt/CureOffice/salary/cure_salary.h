#ifndef CURESALARY_H
#define CURESALARY_H

#include <QWidget>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QGroupBox>
//#include <QProgressDialog>
#include <QtXlsx/QtXlsx>

#include "xlsx/xlsx_sheet_model.h"
#include "cure_email_dialog.h"

QTXLSX_USE_NAMESPACE

class CureSalary : public QWidget
{
    Q_OBJECT
public:
    enum enumStateSenderCheck {
        State_UnChecked = 0,
        State_Check_Failed = 1,
        State_Check_Success
    };

    explicit CureSalary(QWidget *parent = 0);
    virtual ~CureSalary();

signals:

private slots:
    void slotOpenExcel();
    void slotDoTableViewFilter(const QString &strText);
    void slotCheckEmailSenderValid();
    void slotEmailSenderDataChanged(const QString &strText);
    void slotSendEmail();
    void slotDialogSendStart();
    void slotDialogSendCancel();
public slots:
private:
    void initWidgets();
    void freeXlsxDocument();
    void initProgressDialog(const int nMaxValue);
    bool writePersonalInfoToFile(const int index, Format &format, QString &strOutFilePath, QString &strName, int &nIndexNumber);
    void saveSalaryExcelHead(Worksheet *pWorksheet);
    bool findEmailAddressFromExcel(const Worksheet* pWorksheet, const int &nIndexNumber, const QString &strName, QString &strAddress);
    void makeAndSendEmailData();
    Format getEmailDataFormat();
    bool sendEmail(const QString &strReceiver, const QString &strReceiverName, const QString &strAttachFile);
private:
    /* open excel */
    QLabel *m_pLabelOpenExcel;
    QLineEdit *m_pEditOpenedExcel;
    QPushButton *m_pButtonOpenExcel;

    /* email sender config */
    QGroupBox *m_pGroupSender;
    QLabel *m_pLabelSenderAddress;
    QLabel *m_pLabelSenderPasswd;
    QLabel *m_pLabelSMTPServer;
    QLabel *m_pLabelSMTPPort;
    QLabel *m_pLabelSenderValid;

    QLineEdit *m_pEditSenderAddress;
    QLineEdit *m_pEditSenderPasswd;
    QLineEdit *m_pEditSMTPServer;
    QLineEdit *m_pEditSMTPPort;
    QPushButton *m_pButtonSenderValidCheck;

    /* tableview filter */
    QLabel *m_pLabelFilter;
    QLineEdit *m_pEditFilter;
    QLabel *m_pLabelHRName;
    QLineEdit *m_pEditHRName;
    QPushButton *m_pButtonSendEmail;
    QHBoxLayout *m_pHLayoutFilter;

    QTableView *m_pTableExcel;
    Document *m_pXlsxDoc;

    QVBoxLayout *m_pVLayoutMain;
    QHBoxLayout *m_pHLayoutOpenExcel;
    CureEmailDialog *m_pProgressDialog;
    bool m_bStateOpenExcel;
    enumStateSenderCheck m_nStateSenderCheck; /* 发件箱有效确认 */
    bool m_bEmailCanceled;

};

#endif // CURESALARY_H
