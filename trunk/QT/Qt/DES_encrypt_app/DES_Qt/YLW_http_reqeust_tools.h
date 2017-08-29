#ifndef CHTTPREQEUSTTOOLS_H
#define CHTTPREQEUSTTOOLS_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QTableWidget>
#include <QCheckBox>
#include <QLayout>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class CHttpReqeustTools : public QDialog
{
    Q_OBJECT
public:
    enum enumRequestType {
        TYPE_REQUEST_AD,
        TYPE_REQUEST_UPDATE
    };
    enum enumServerType {
        TYPE_SERVER_DNET,
        TYPE_SREVER_JAVA
    };


    explicit CHttpReqeustTools(QWidget *parent = 0);
    virtual ~CHttpReqeustTools();

signals:
    void sigRequestFinished(const QString &strReply);
private slots:
    void slotReqestTypeChanged(int index);
    void slotRequestServerChanged(int index);
    void slotStartRequest();
    void slotChangeDisplay();
    void slotMacChanged(const QString &);
    void slotSWVerChanged(const QString &);
    void slotReplyFinished();
    void slotReadyRead();
    void slotNetError(QNetworkReply::NetworkError code);
    void slotSslErrors(const QList<QSslError> & errors);
    void slotHandReplyData(const QString &strReply);

private:
    QLabel *m_pLabelRequetType;
    QLabel *m_pLabelServerType;
    QLabel *m_pLabelMac;
    QLabel *m_pLabelRequestResult;
    QLabel *m_pLabelCurrentHWVersion;
    QLabel *m_pLabelCurrentSWVersion;

    QPushButton *m_pButtonRequest;
    QCheckBox *m_pCheckBoxDisplay;

    QComboBox *m_pComboBoxRequestType;
    QComboBox *m_pComboBoxServerType;
    QComboBox *m_pComboBoxHWVersion;
    QLineEdit *m_pLineEditServer;
    QLineEdit *m_pLineEditMac;
    QLineEdit *m_pLineEditSWVer;
    QTextEdit *m_pTextEditAD;
    QTextEdit *m_pTextEditUpdate;
    QTableWidget *m_pTableWidgetAD;
    QTableWidget *m_pTableWidgetUpdate;

    QVBoxLayout *m_pVLayoutMain;
    QHBoxLayout *m_pHLayoutType;
    QHBoxLayout *m_pHLayoutMac;
    QHBoxLayout *m_pHLayoutVersion;
    QHBoxLayout *m_pHLayoutResult;

    QString m_strMac;
    bool m_bIsMacValid;
    bool m_bIsSWVerValid;

    QNetworkAccessManager *m_pAccessManager;
    QNetworkReply *m_pNetworkReply;

private:
    void initTableWigets();
    void getRequestData(QString &strData);
    void setTableWidgetDataAD(const QString &strData);
    void setTableWidgetDataUpdate(const QString &strData);
};

#endif // CHTTPREQEUSTTOOLS_H
