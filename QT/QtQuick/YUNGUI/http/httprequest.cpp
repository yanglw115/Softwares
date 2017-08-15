#include "httprequest.h"

HttpRequest::HttpRequest(QObject *parent)
    : QObject(parent)
    , m_pNetManager(0)
    , m_pNetReply(0)
    , m_requestNow(0)
{
    m_contentTypeHeader = "application/json";
}

HttpRequest::~HttpRequest()
{
    if (m_pNetReply) {
        m_pNetReply->deleteLater();
        m_pNetReply = 0;
    }

    if (m_requestNow) {
        delete m_requestNow;
        m_requestNow = 0;
    }

    if (m_pNetManager) {
        delete m_pNetManager;
        m_pNetManager = 0;
    }

    m_reqInfoQueue.clear();
}

bool HttpRequest::init()
{
    m_pNetManager = new QNetworkAccessManager(this);
    // QTimer::singleShot(0, this, SLOT(getRequest()));
    return true;
}

bool HttpRequest::startRequest(const QString &reqUrl, const QString &reqData, const requestType_t type, const QString &requestID)
{
    struReqInfo_t *reqInfo = new struReqInfo_t();
    reqInfo->reqUrl = reqUrl;
    reqInfo->reqData = reqData;
    reqInfo->reqType = type;
    reqInfo->reqID = requestID;

    enqueue(reqInfo);
    QTimer::singleShot(0, this, SLOT(getRequest()));
    return true;
}

bool HttpRequest::request()
{
    if (postRequest(m_requestNow->reqUrl, m_requestNow->reqData)) {
        connect(m_pNetReply, SIGNAL(finished()), this, SLOT(replyFinished()));
        return true;
    }

    return false;
}

bool HttpRequest::enqueue(struReqInfo_t *reqInfo)
{
    if (0 == reqInfo) {
        qCritical("the request to add is null");
        return false;
    }

    m_reqInfoQueue.enqueue(reqInfo);
    qDebug()<<"queue count:"<<m_reqInfoQueue.count();

    return true;
}

struReqInfo_t *HttpRequest::dequeue()
{
    struReqInfo_t *reqInfo = 0;
    if (m_reqInfoQueue.empty()) {
        qDebug()<<"empty Now";
        return 0;
    }

    reqInfo = m_reqInfoQueue.dequeue();

    if (0 == reqInfo) {
        qCritical()<<"the gained request is null";
        return 0;
    }

    return reqInfo;
}

bool HttpRequest::postRequest(QString reqUrl, const QString &reqData)
{
//    if (reqData.isEmpty() || reqData.isNull()) {
//      qCritical()<<"http请求发送处理 请求数据为空"<<Q_HTTP_REQ_TYPE_STRING[httpReqType];
//        return false;
//    }

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(m_contentTypeHeader));
    request.setUrl(QUrl(reqUrl));

    if (0 == m_pNetManager)
        return false;

    m_pNetReply = m_pNetManager->post(request, reqData.toLocal8Bit()/*QString(reqData.toUtf8().data()).toLatin1()*/);
    connect(m_pNetReply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(httpRequestError(QNetworkReply::NetworkError)));

    return true;
}

void HttpRequest::getRequest()
{
    if (0 == m_requestNow) {
        m_requestNow = dequeue();
        if (0 != m_requestNow) {
            qDebug()<<"get request successfully";
            QTimer::singleShot(0, this, SLOT(request()));
        } else {
            qDebug()<<"get request failed, no request available now maybe";
        }
    } else {
        qDebug()<<"request in executing";
    }
}

void HttpRequest::replyFinished()
{
    if (0 == m_pNetReply) {
        qCritical()<<"replyFinished, networkReply is NULL";
        return;
    }

    // 发出http请求返回处理信号到主窗口 让主窗口处理http返回请求
    if (QNetworkReply::NoError == m_pNetReply->error()) {
        qDebug()<<"replyFinished, http request return normally";
        emit dealHttpResponse(m_pNetReply->readAll(), true, m_requestNow->reqType, m_requestNow->reqID);
    } else {
        qCritical()<<"replyFinished, http request return not normally error code:"<<m_pNetReply->error();
        emit dealHttpResponse(QByteArray(""), false, m_requestNow->reqType, m_requestNow->reqID);
    }

    m_pNetReply->deleteLater();
    m_pNetReply = 0;
    delete m_requestNow;
    m_requestNow = 0;

    QTimer::singleShot(0, this, SLOT(getRequest()));
}

void HttpRequest::httpRequestError(QNetworkReply::NetworkError networkError)
{
    qCritical()<<"http request error occur error code:"<<networkError;
    m_pNetReply->deleteLater();
    m_pNetReply = 0;
    delete m_requestNow;
    m_requestNow = 0;
}
