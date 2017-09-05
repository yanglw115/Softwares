#include "YLW_http_reqeust_tools.h"
#include "YLW_vs_char_set.h"
#include "YLW_key_value.h"
#include "YLW_utils.h"

#include <QtDebug>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>
#include <QListView>

static const QString g_strServersAD[2] = {"http://sume.daboowifi.net/v2/api/task",
                                        "http://openapi.openoo.com/forward/api/rest/router"};

static const QString g_strServersUpdate[2] = {"http://router.daboowifi.net/scale/upgrade/check",
                                            "http://openapi.openoo.com/forward/api/rest/router"};

static const QString g_strTimeFormat = "yyyy-MM-dd hh:mm:ss";

/**  JAVA相关接口 **/
#define METHOD_AD_TASK "youle.machine.ad.publishPlan"
#define METHOD_UPGRADE_CHECK "youle.version.update.check"

#define APPKEY		"W08a1G9fxIoXR9RW808a1G9fxIoXR9Rx"
#define HTTPVER		"1.0"
#define FORMAT		"json"

#define SECRETKEY  "dasdfasdfghjkqwertyughjkqwertyu"
static const char ClientId[] = "sumenet_54d1c89ee30a29bfcd36cd36dd34rrs";

CHttpReqeustTools::CHttpReqeustTools(QWidget *parent)
    : QDialog(parent)
    , m_bIsMacValid(false)
    , m_bIsSWVerValid(false)
{
    m_pLabelRequetType = new QLabel(tr("任务类型:"), this);
    m_pLabelServerType = new QLabel(tr("服务器类型:"), this);
    m_pLabelMac = new QLabel(tr("MAC:"), this);
    m_pLabelRequestResult = new QLabel(tr("请求结果:"), this);
    m_pLabelCurrentHWVersion = new QLabel(tr("硬件版本号:"), this);
    m_pLabelCurrentSWVersion = new QLabel(tr("软件版本号:"), this);
    m_pLabelHash = new QLabel(tr("Hash: "), this);

    m_pButtonRequest = new QPushButton(tr("开始请求"), this);
    connect(m_pButtonRequest, SIGNAL(clicked(bool)), this, SLOT(slotStartRequest()));
    m_pCheckBoxDisplay = new QCheckBox(tr("显示转换"), this);
    connect(m_pCheckBoxDisplay, SIGNAL(stateChanged(int)), this, SLOT(slotChangeDisplay()));

    m_pComboBoxRequestType = new CMyComboBox(this);
    m_pComboBoxRequestType->addItems(QStringList() << tr("广告任务") << tr("升级任务"));
    m_pComboBoxRequestType->setCurrentIndex(TYPE_REQUEST_AD);
    m_pComboBoxServerType = new CMyComboBox(this);
    m_pComboBoxServerType->addItems(QStringList() << tr(".NET") << tr("JAVA"));
    m_pComboBoxHWVersion = new CMyComboBox(this);
    m_pComboBoxHWVersion->addItems(QStringList() << tr("HealthWeighingTr1") << tr("HealthWeighingTraingle") << tr("HealthWeighingFans") << tr("HealthWeighingVS"));
    m_pComboBoxHWVersion->setCurrentIndex(0);
    connect(m_pComboBoxRequestType, SIGNAL(currentIndexChanged(int)), this, SLOT(slotReqestTypeChanged(int)));
    connect(m_pComboBoxServerType, SIGNAL(currentIndexChanged(int)), this, SLOT(slotRequestServerChanged(int)));

    m_pLineEditServer = new QLineEdit(this);
    m_pLineEditServer->setText(g_strServersAD[0]);
    m_pLineEditMac = new QLineEdit(this);
    m_pLineEditSWVer = new QLineEdit(this);
    m_pLineEditHash = new QLineEdit(this);
    m_pLineEditHash->setReadOnly(true);
    connect(m_pLineEditMac, SIGNAL(textChanged(QString)), this, SLOT(slotMacChanged(QString)));
    connect(m_pLineEditSWVer, SIGNAL(textChanged(QString)), this, SLOT(slotSWVerChanged(QString)));

    m_pTextEditAD = new QTextEdit(this);
    m_pTextEditAD->setReadOnly(true);
    m_pTextEditUpdate = new QTextEdit(this);
    m_pTextEditUpdate->setReadOnly(true);

    m_pTableWidgetAD = new QTableWidget(this);
    m_pTableWidgetUpdate = new QTableWidget(1, 2, this);
    m_pTableWidgetAD->setColumnCount(7);
    m_pTableWidgetAD->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pTableWidgetAD->setSortingEnabled(true);
    m_pTableWidgetUpdate->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pTableWidgetUpdate->setSortingEnabled(true);

    m_pHLayoutType = new QHBoxLayout;
    m_pHLayoutType->addWidget(m_pLabelRequetType);
    m_pHLayoutType->addWidget(m_pComboBoxRequestType);
    m_pHLayoutType->addWidget(m_pLabelServerType);
    m_pHLayoutType->addWidget(m_pComboBoxServerType);
    m_pHLayoutType->addWidget(m_pLineEditServer);

    m_pHLayoutMac = new QHBoxLayout;
    m_pHLayoutMac->addWidget(m_pLabelMac);
    m_pHLayoutMac->addWidget(m_pLineEditMac);
    m_pHLayoutMac->addWidget(m_pButtonRequest);
    m_pHLayoutMac->addStretch();

    m_pHLayoutVersion = new QHBoxLayout;
    m_pHLayoutVersion->addWidget(m_pLabelCurrentHWVersion);
    m_pHLayoutVersion->addWidget(m_pComboBoxHWVersion);
    m_pHLayoutVersion->addWidget(m_pLabelCurrentSWVersion);
    m_pHLayoutVersion->addWidget(m_pLineEditSWVer);
    m_pHLayoutVersion->addStretch();

    m_pHLayoutResult = new QHBoxLayout;
    m_pHLayoutResult->addWidget(m_pLabelRequestResult);
    m_pHLayoutResult->addWidget(m_pCheckBoxDisplay);
    m_pHLayoutResult->addStretch();
    m_pHLayoutResult->addWidget(m_pLabelHash);
    m_pHLayoutResult->addWidget(m_pLineEditHash);
    //m_pHLayoutResult->addStretch();

    m_pVLayoutMain = new QVBoxLayout(this);
    m_pVLayoutMain->addSpacing(10);
    m_pVLayoutMain->addLayout(m_pHLayoutType);
    m_pVLayoutMain->addSpacing(10);
    m_pVLayoutMain->addLayout(m_pHLayoutMac);
    m_pVLayoutMain->addSpacing(10);
    m_pVLayoutMain->addLayout(m_pHLayoutVersion);
    m_pVLayoutMain->addSpacing(10);
    m_pVLayoutMain->addLayout(m_pHLayoutResult);
    m_pVLayoutMain->addSpacing(10);
    m_pVLayoutMain->addWidget(m_pTextEditAD);
    m_pVLayoutMain->addWidget(m_pTextEditUpdate);
    m_pVLayoutMain->addWidget(m_pTableWidgetAD);
    m_pVLayoutMain->addWidget(m_pTableWidgetUpdate);

    m_pComboBoxHWVersion->setEnabled(false);
    m_pLineEditSWVer->setEnabled(false);
    m_pButtonRequest->setEnabled(false);
    m_pCheckBoxDisplay->setEnabled(false);

    m_pTextEditAD->show();
    m_pTextEditUpdate->hide();
    m_pTableWidgetAD->hide();
    m_pTableWidgetUpdate->hide();

    initTableWigets();

    m_pAccessManager = new QNetworkAccessManager(this);
    connect(this, SIGNAL(sigRequestFinished(QString)), this, SLOT(slotHandReplyData(QString)));
}

CHttpReqeustTools::~CHttpReqeustTools()
{

}

void CHttpReqeustTools::slotReqestTypeChanged(int index)
{
    if (TYPE_REQUEST_AD == index) {
        m_pLineEditServer->setText(g_strServersAD[m_pComboBoxServerType->currentIndex()]);
        slotMacChanged(m_pLineEditMac->text());
    } else {
        m_pLineEditServer->setText(g_strServersUpdate[m_pComboBoxServerType->currentIndex()]);
        if (m_pLineEditSWVer->text().toInt() <= 0) {
            m_pButtonRequest->setEnabled(false);
        }
        m_pLabelHash->hide();
        m_pLineEditHash->hide();
    }
    m_pComboBoxHWVersion->setEnabled(index);//这里使用index来代替了true与false
    m_pLineEditSWVer->setEnabled(index);
}

void CHttpReqeustTools::slotRequestServerChanged(int index)
{
    if (TYPE_SERVER_DNET == m_pComboBoxRequestType->currentIndex()) {
        m_pLineEditServer->setText(g_strServersAD[index]);
    } else {
        m_pLineEditServer->setText(g_strServersUpdate[index]);
    }
}

void CHttpReqeustTools::slotStartRequest()
{
    QString strData;
    getRequestData(strData);

    QUrl url(m_pLineEditServer->text());
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded;charset=utf-8");
    m_pNetworkReply = m_pAccessManager->post(request, strData.toLocal8Bit());
    if (NULL == m_pNetworkReply) {
        qWarning() << "QNetworkAccessManager post failed!";
        return;
    }

//    if (QSslSocket::supportsSsl()) {
//        qDebug() << "SSL supported!";
//    } else {
//        qDebug() << "SSL not supported!";
//    }

    m_pCheckBoxDisplay->setEnabled(false);
    m_pButtonRequest->setEnabled(false);
    this->setEnabled(false);

    connect(m_pNetworkReply, SIGNAL(finished()), this, SLOT(slotReplyFinished()));
    connect(m_pNetworkReply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(m_pNetworkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotNetError(QNetworkReply::NetworkError)));
    connect(m_pNetworkReply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
}

void CHttpReqeustTools::slotChangeDisplay()
{
    if (m_pCheckBoxDisplay->isChecked()) {
        if (TYPE_REQUEST_AD == m_pComboBoxRequestType->currentIndex()) {
            m_pTextEditAD->hide();
            m_pTextEditUpdate->hide();
            m_pTableWidgetUpdate->hide();
            m_pTableWidgetAD->show();
        } else {
            m_pTextEditAD->hide();
            m_pTextEditUpdate->hide();
            m_pTableWidgetUpdate->show();
            m_pTableWidgetAD->hide();
        }
    } else {
        if (TYPE_REQUEST_AD == m_pComboBoxRequestType->currentIndex()) {
            m_pTextEditAD->show();
            m_pTextEditUpdate->hide();
            m_pTableWidgetUpdate->hide();
            m_pTableWidgetAD->hide();
        } else {
            m_pTextEditAD->hide();
            m_pTextEditUpdate->show();
            m_pTableWidgetUpdate->hide();
            m_pTableWidgetAD->hide();
        }
    }
}

void CHttpReqeustTools::slotMacChanged(const QString &strMac)
{
    bool bIsValid = false;
    QString strMacTemp = strMac.trimmed();
    strMacTemp = strMacTemp.toLower();
    strMacTemp.replace('-', ':');
    if (strMacTemp.length() == strlen("00:00:00:00:00:00")) {
        QByteArray byteArray = strMacTemp.toLocal8Bit();
        qDebug() << "byteArray: " << byteArray;
        bIsValid = true;
        for (int i = 0; i < byteArray.length(); ++i) {
            char temp = byteArray.at(i);
            if ((i + 1) % 3 == 0) {
                if (temp != ':') {
                    bIsValid = false;
                    break;
                } else {
                    continue;
                }
            }
            if (!isdigit(temp) && (temp < 'a') || (temp > 'f')) {
                bIsValid = false;
                break;
            }
        }
    }

    QPalette palette;
    if (bIsValid) {
        m_bIsMacValid = true;
        m_pLineEditMac->setText(strMacTemp);
        m_pLineEditMac->setPalette(palette);
        palette.setColor(QPalette::Text, Qt::black);
        if (TYPE_REQUEST_AD == m_pComboBoxRequestType->currentIndex()) {
            m_pButtonRequest->setEnabled(true);
        } else {
            if (m_bIsSWVerValid) {
                m_pButtonRequest->setEnabled(true);
            } else {
                m_pButtonRequest->setEnabled(false);
            }
        }
    } else {
        m_bIsMacValid = false;
        palette.setColor(QPalette::Text, Qt::red);
        m_pLineEditMac->setPalette(palette);
        m_pButtonRequest->setEnabled(false);
    }
}

void CHttpReqeustTools::slotSWVerChanged(const QString &strSWVer)
{
    QPalette palette;
    QString strMacTemp = strSWVer.trimmed();
    if (strMacTemp.toInt() > 0) {
        m_bIsSWVerValid = true;
        palette.setColor(QPalette::Text, Qt::black);
        m_pLineEditSWVer->setPalette(palette);
        m_pLineEditSWVer->setText(strMacTemp);
        if (m_bIsMacValid) {
            m_pButtonRequest->setEnabled(true);
        } else {
            m_pButtonRequest->setEnabled(false);
        }
    } else {
        palette.setColor(QPalette::Text, Qt::red);
        m_pLineEditSWVer->setPalette(palette);
        m_pButtonRequest->setEnabled(false);
        m_bIsSWVerValid = false;
    }
}

void CHttpReqeustTools::slotReplyFinished()
{
    this->setEnabled(true);
    QString strReply;
    if (TYPE_REQUEST_AD == m_pComboBoxRequestType->currentIndex()) {
        m_pLabelHash->show();
        m_pLineEditHash->show();
        m_pLineEditHash->clear();
    }
    if (m_pNetworkReply && (QNetworkReply::NoError == m_pNetworkReply->error())) {
        strReply = m_pNetworkReply->readAll();
        m_pNetworkReply->deleteLater();
        m_pNetworkReply = NULL;
        emit sigRequestFinished(strReply);
    }
    m_pButtonRequest->setEnabled(true);
    m_pCheckBoxDisplay->setEnabled(true);
}

void CHttpReqeustTools::slotReadyRead()
{

}

void CHttpReqeustTools::slotNetError(QNetworkReply::NetworkError code)
{
    QString strError = QString("%1%2").arg(tr("Network error:")).arg(code);
    qWarning() << strError;
    emit sigRequestFinished(strError);
}

void CHttpReqeustTools::slotSslErrors(const QList<QSslError> & errors)
{
    Q_UNUSED(errors);
    qWarning() << "Network request ssl error!";
}

void CHttpReqeustTools::slotHandReplyData(const QString &strReply)
{
    if (TYPE_REQUEST_AD == m_pComboBoxRequestType->currentIndex()) {
        m_pTextEditAD->clear();
        m_pTextEditAD->setText(strReply);
        setTableWidgetDataAD(strReply);
    } else {
        m_pTextEditUpdate->clear();
        m_pTextEditUpdate->setText(strReply);
        setTableWidgetDataUpdate(strReply);
    }
    slotChangeDisplay();
}

void CHttpReqeustTools::initTableWigets()
{
    m_pTableWidgetAD->setHorizontalHeaderLabels(QStringList() << tr("Gid") << tr("Type") << tr("StartTime") << tr("EndTime") << tr("Duration") << tr("MD5") << tr("Url"));
    m_pTableWidgetUpdate->setHorizontalHeaderLabels((QStringList() << tr("Version") << tr("Url")));
    m_pLabelHash->hide();
    m_pLineEditHash->hide();
}

void CHttpReqeustTools::getRequestData(QString &strData)
{
    QJsonObject objJson;
    QJsonDocument docJson;
    if (TYPE_REQUEST_AD == m_pComboBoxRequestType->currentIndex()) {
        if (TYPE_SERVER_DNET == m_pComboBoxServerType->currentIndex()) {
            objJson.insert(tr("ucode"), QJsonValue(m_pLineEditMac->text()));
        } else {
            getADTaskRequestBody(strData);
            return;
        }
    } else {
        if (TYPE_SERVER_DNET == m_pComboBoxServerType->currentIndex()) {
            objJson.insert(tr("hardware"), QJsonValue(m_pComboBoxHWVersion->currentText()));
            objJson.insert(tr("firmware"), QJsonValue(tr("winxp")));
            objJson.insert(tr("package"), QJsonValue(tr("1.12")));
            objJson.insert(tr("version"), QJsonValue(m_pLineEditSWVer->text()));
            objJson.insert(tr("wifiid"), QJsonValue(m_pLineEditMac->text()));
            objJson.insert(tr("ukey"), QJsonValue(tr("4f04313af18f3803:5ac11055bcb73aa4")));
            objJson.insert(tr("key"), QJsonValue(tr("NGYwNDMxM2FmMThmMzgwMzo1YWMxMTA1NWJjYjczYWE0")));
            objJson.insert(tr("ucode"), QJsonValue(m_pLineEditMac->text() + tr(":Route")));
        } else {
            getUpdateTaskRequestBody(strData);
            return;
        }
    }
    docJson.setObject(objJson);
    strData = docJson.toJson(QJsonDocument::Indented);
}

void CHttpReqeustTools::setTableWidgetDataAD(const QString &strData)
{
    QJsonObject objRoot = QJsonDocument::fromJson(strData.toLatin1()).object();
    QJsonObject objResult;
    QJsonArray arrayTask;

    QString strHash = (objRoot["hash"].isUndefined()? "NULL": objRoot["hash"].toString());
    m_pLineEditHash->setText(strHash);

    if (TYPE_SERVER_DNET == m_pComboBoxServerType->currentIndex()) {
        objResult = (objRoot["result"].isUndefined()? QJsonObject(): objRoot["result"].toObject());
        arrayTask = (objResult.isEmpty()? QJsonArray(): objResult["task"].toArray());
    } else {
        arrayTask = (objRoot["list"].isUndefined()? QJsonArray(): objRoot["list"].toArray());
    }
    int nTaskCount = (arrayTask.isEmpty()? 0: arrayTask.size());

    m_pTableWidgetAD->clearContents();
    if (nTaskCount > 0) {
        m_pTableWidgetAD->setRowCount(nTaskCount);
        for (int i = 0; i < nTaskCount; ++i) {
            QJsonObject objTemp = arrayTask[i].toObject();
            if (objTemp.isEmpty()) {
                continue;
            }
            QString strGid = (objTemp["gid"].isUndefined()? "NULL": objTemp["gid"].toString());
            QString strUrl = (objTemp["url"].isUndefined()? "NULL": objTemp["url"].toString());
            QString strMd5 = (objTemp["md5"].isUndefined()? "NULL": objTemp["md5"].toString());
            int nStartTime = (objTemp["playTimeStart"].isUndefined()? -1: objTemp["playTimeStart"].toInt());
            int nEndTime = (objTemp["playTimeEnd"].isUndefined()? -1: objTemp["playTimeEnd"].toInt());
            int nType = (objTemp["type"].isUndefined()? -1: objTemp["type"].toInt());
            int nDuration = (objTemp["duration"].isUndefined()? -1: objTemp["duration"].toInt());
            m_pTableWidgetAD->setItem(i, 0, new QTableWidgetItem(strGid));
            m_pTableWidgetAD->setItem(i, 1, new QTableWidgetItem(tr("%1").arg(nType)));
            m_pTableWidgetAD->setItem(i, 2, new QTableWidgetItem(tr("%1").arg((nStartTime == -1? "NULL": QDateTime::fromTime_t(nStartTime).toString(g_strTimeFormat)))));
            m_pTableWidgetAD->setItem(i, 3, new QTableWidgetItem(tr("%1").arg((nEndTime == -1? "NULL": QDateTime::fromTime_t(nEndTime).toString(g_strTimeFormat)))));
            m_pTableWidgetAD->setItem(i, 4, new QTableWidgetItem(tr("%1").arg(nDuration)));
            m_pTableWidgetAD->setItem(i, 5, new QTableWidgetItem(strMd5));
            m_pTableWidgetAD->setItem(i, 6, new QTableWidgetItem(strUrl));
        }
    }
}

void CHttpReqeustTools::setTableWidgetDataUpdate(const QString &strData)
{
    QJsonObject objRoot = QJsonDocument::fromJson(strData.toLatin1()).object();
    QJsonObject objVersion;
    if (TYPE_SERVER_DNET == m_pComboBoxServerType->currentIndex()) {
        objVersion = (objRoot["version"].isUndefined()? QJsonObject(): objRoot["version"].toObject());
    } else {
        objVersion = objRoot;
    }
    QString strVer = "NULL";
    QString strUrl = "NULL";
    if (!objVersion.isEmpty()) {
        strVer = (objVersion["vernum"].isUndefined()? "NULL": objVersion["vernum"].toString());
        strUrl = (objVersion["url"].isUndefined()? "NULL": objVersion["url"].toString());
    }

    m_pTableWidgetUpdate->clearContents();
    m_pTableWidgetUpdate->setItem(0, 0, new QTableWidgetItem(strVer));
    m_pTableWidgetUpdate->setItem(0, 1, new QTableWidgetItem(strUrl));
}

void CHttpReqeustTools::getADTaskRequestBody(QString &strData)
{
    KeyValue kv;
    QString strSign;
    strData.clear();
    kv.putString("appKey", APPKEY);
    kv.putString("method", METHOD_AD_TASK);
    kv.putString("format", FORMAT);
    kv.putString("ver", HTTPVER);
    kv.putString("mac", m_pLineEditMac->text());
    kv.putString("timestamp", QDateTime::currentDateTime().toString(g_strTimeFormat));

    strSign = SECRETKEY + kv.toQString() + SECRETKEY;
    strSign = CUtils::calculateMD5ForString(strSign);
    kv.putString("sign", strSign);

    kv.setConnector("=");
    kv.setDelimiter("&");

    strData = kv.toQString();
}

void CHttpReqeustTools::getUpdateTaskRequestBody(QString &strData)
{
    KeyValue kv;
    QString strSign;
    strData.clear();
    kv.putString("appKey", APPKEY);
    kv.putString("method", METHOD_UPGRADE_CHECK);
    kv.putString("format", FORMAT);
    kv.putString("ver", HTTPVER);
    kv.putString("wifiid", m_pLineEditMac->text());
    kv.putString("version", m_pLineEditSWVer->text());
    kv.putString("timestamp", QDateTime::currentDateTime().toString(g_strTimeFormat));

    strSign = SECRETKEY + kv.toQString() + SECRETKEY;
    strSign = CUtils::calculateMD5ForString(strSign);
    kv.putString("sign", strSign);

    kv.setConnector("=");
    kv.setDelimiter("&");

    strData = kv.toQString();
}

