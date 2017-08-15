#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

//#include "httprequest_global.h"
#include <QObject>
#include <QtCore/qglobal.h>
#include <QtNetwork>

//#if defined(HTTPREQUEST_LIBRARY)
//#  define HTTPREQUESTSHARED_EXPORT Q_DECL_EXPORT
//#else
//#  define HTTPREQUESTSHARED_EXPORT Q_DECL_IMPORT
//#endif
#define HTTPREQUESTSHARED_EXPORT

typedef enum enumRequstType {
    TYPE_REQUEST_GL = 0,
    TYPE_REQUEST_PIC = 1,
    TYPE_UPLOAD_SOLD_INFO
} requestType_t;

typedef struct struReqInfo struReqInfo_t;
struct struReqInfo
{
    QString reqUrl;
    QString reqData;
    QString contentTypeHeader;
    enumRequstType reqType;
    QString reqID;

    struReqInfo() {
        reqUrl = QString("");
        reqData = QString("");
        contentTypeHeader = QString("");
    }
};

class HTTPREQUESTSHARED_EXPORT HttpRequest : public QObject
{

public:
    Q_OBJECT
public:
    explicit HttpRequest(QObject *parent = 0);
    ~HttpRequest();

    /**************************************************************************
     * Function: Init
     * Description: 初始化
     * Input: 无
     * Output: 无
     * Return: true,成功;false,失败
     **************************************************************************/
    bool init();

    bool startRequest(const QString &reqUrl, const QString &reqData, const requestType_t type, const QString &requestID);

signals:
    /**************************************************************************
     * Function: dealHttpResponse
     * Description: http请求完成返回数据处理信号 从当前模板发送到主窗口模板
     * Input: httpReqType,http请求类型;strResponseData,http回复数据;bRetSucc,解析是否成功
     * Output: 无
     * Return: 无
     **************************************************************************/
    void dealHttpResponse(const QByteArray &responseData, bool bRetSucc, const requestType_t type, const QString &requestID);

public slots:

private:
    /**************************************************************************
     * Function: enqueue
     * Description: 插入数据
     * Input: reqInfo, reqinfo
     * Output: 无
     * Return: true,成功;false,失败
     **************************************************************************/
    bool enqueue(struReqInfo_t *reqInfo);

    /**************************************************************************
     * Function: dequeue
     * Description: 获取队列头数据
     * Input: 无
     * Output: 无
     * Return: 队列头数据指针
     **************************************************************************/
    struReqInfo_t *dequeue();

    /**************************************************************************
     * Function: postRequest
     * Description: http发送请求统一处理
     * Input: httpReqType,http请求类型;strReqData,http请求数据
     * Output: 无
     * Return: true,成功;false,失败
     **************************************************************************/
    bool postRequest(QString reqUrl, const QString &reqData);

    /**************************************************************************
     * Function: replyFinished
     * Description: http请求完成返回统一处理
     * Input: httpReqType,http请求类型
     * Output: 无
     * Return: true,成功;false,失败
     **************************************************************************/
    void replyFinished(int httpReqType);

private slots:
    // 获取任务
    void getRequest();
    bool request();

    void replyFinished();

    // http请求错误响应SLOT
    // This signal is emitted when the reply detects an error in processing.
    // The finished signal will probably follow,indicating that the connection is over.
    // 出错之后会自己到finished() 所以不需要特殊处理
    // 并且这个地方还不能调用netreply的close或者abort 不然在finished()响应会报内部错误重复调用
    void httpRequestError(QNetworkReply::NetworkError networkError);

private:
    QNetworkAccessManager *m_pNetManager;
    QNetworkReply *m_pNetReply;
    QString m_contentTypeHeader;
    struReqInfo_t *m_requestNow;
    QQueue<struReqInfo_t *> m_reqInfoQueue;
};

#endif // HTTPREQUEST_H
