
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QThread>
#include <QtDebug>

#include "YG_goods_info.h"
#include "YG_utils.h"

CGoodsInfo *g_pGoodsInfo                            = NULL;
static const QString g_strDatabaseName              = "share/adconfig";
static const QString g_strSqlDriverName             = "QSQLITE";
static const QString g_strConnectName               = "YG_APP_GOODS";
static const QString g_strGoodsInfoTableName        = "goodsinfo";
static const QString g_strVersionTableName          = "version";

static  QString g_strHashGoodslist                  = "";

#ifdef __cplusplus
extern "C" {
/**************************************************************************
 * Function: createGoodsInfoInstance
 * Description: 创建商品信息的实例。在QML中通过商品管理类引用该商品信息实例指针进行商品信息
 * 的相关操作，因为商品信息类在整个程序运行当中只有并且只能有一个实例化的对象，该对象也不能放在
 * 商品管理类当中，因为商品管理在QML中会有多个实例；又为了能够让外部其他类能够访问该类，使用一
 * 个全局的指针供外部引用。
 * Input: 父对象，便于父对象销毁时将其一起销毁掉。本工程暂时没有用到。
 * Output: 无
 * Return: 创建成功返回true,失败则返回false。如果返回失败，则整个程序将直接退出。
 *************************************************************************/
bool createGoodsInfoInstance(QObject *parent)
{
    g_pGoodsInfo = new CGoodsInfo;
    QThread *pThreadGoodsInfo = new QThread(parent);
    if (!g_pGoodsInfo || !pThreadGoodsInfo) {
        qCritical() << "Create goods info instance failed!";
        return false;
    }
    g_pGoodsInfo->moveToThread(pThreadGoodsInfo);
    QObject::connect(pThreadGoodsInfo, SIGNAL(started()), g_pGoodsInfo, SLOT(slotStartInit()));
    pThreadGoodsInfo->start();

    return true;
}

/**************************************************************************
 * Function: getGoodsInfoInstance
 * Description: 获取商品信息的实例指针
 * Input: 无
 * Output: 无
 * Return: 返回商品信息实例指针。该接口在商品管理类的构造函数中用到，用于初始化商品管理类
 * 的成员。
 *************************************************************************/
CGoodsInfo *getGoodsInfoInstance()
{
    return g_pGoodsInfo;
}
}
#endif

CGoodsInfo::CGoodsInfo(QObject *parent)
    : QObject(parent)
    , m_pCoinAcceptor(NULL)
    , m_pCoinPayout(NULL)
    , m_pNoteAcceptor(NULL)
    , m_pCardReadr(NULL)
    , m_bIsDBEmpty(true)
    , m_pHttpRequest(NULL)
{
    m_mapGoods.clear();
    m_struPayInfo.clear();

    /* 实例化硬币、纸币处理设备，这里实例化完成之后即进行了初始化。也可以在使用进再进行初始化，但是需要设备类提供更多的接口便于管理 */
    m_pCoinAcceptor = new CCoinAcceptor(this);
    m_pCoinAcceptor->slotInit();

    m_pCoinPayout = new CCoinPayout(this);
    m_pCoinPayout->slotInitPayout();

    m_pNoteAcceptor = new CNoteAcceptor(this);
    m_pNoteAcceptor->slotInit();

    if (PAY_WITH_CARD_READER) { /* 使用读卡器还是制卡器进行支付 */
        m_pCardReadr = new CCardReader(this);
        m_pCardReadr->slotInit();
    } else {
        m_pCardReleaser = new CCardReleaser;
        m_pCardReleaser->slotInit();
    }

}

CGoodsInfo::~CGoodsInfo()
{
    if (m_pCardReleaser) {
        m_pCardReleaser->deleteLater();
    }
}

/**************************************************************************
 * Function: slotStartInit
 * Description: 开始进行初始化。因为商品信息类通过moveToThread方式在线程中进行运行，所以
 * 在线程start之后，通过slotStartInit触发商品信息类的功能开始。该方法在商品信息类的实例中
 * 只调用一次，目的是从本地数据库当中读取货架上的商品信息；然后从服务器上同步最新的商品信息(
 * 如果需要更新本地信息的话，则更新本地信息)；
 * Input: 无
 * Output: 无
 * Return: 无
 *************************************************************************/
void CGoodsInfo::slotStartInit()
{
    initDatabase();

    m_mutex.lock();
    readGoodsListFromDB();
    printGoodsList();
    m_mutex.unlock();

    syncGoodsListFromServer();
}


/**************************************************************************
 * Function: slotHandleHttpRequestResult
 * Description: 处理HTTP请求结果的槽函数。
 * Input:
 *      responseData：HTTP请求得到的数据
 *      bRetSucc: HTTP请求成功与否
 *      type: HTTP请求的类型。因为本程序中所有HTTP请求通过一个对象进行，所以需要根据该
 *          类型进行区分是哪一类请求
 *      requestID: HTTP请求时所用的ID，为了区分每一类请求中的具体请求，使用该ID。特别是
 *          在一次性发起对所有商品图片请求后，处理HTTP请求应该时该ID将很有作用。
 * Output: 无
 * Return: 无
 *************************************************************************/
void CGoodsInfo::slotHandleHttpRequestResult(const QByteArray &responseData, bool bRetSucc,
                                             const requestType_t type, const QString &requestID)
{
    if (!bRetSucc) {
        switch (type) {
        case TYPE_REQUEST_GL:
            qCritical() << "Request goods list from server failed!";
            break;
        case TYPE_REQUEST_PIC:
            qCritical() << "Request picture failed: (ID)" << requestID;
            break;
        case TYPE_UPLOAD_SOLD_INFO:
            qCritical() << "Upload sold goods info failed!";
            break;
        default:
            break;
        }
    } else {
        switch (type) {
        case TYPE_REQUEST_GL: /* 商品列表请求 */
            qDebug() << "Request goods list info result:";
            qDebug() << responseData;
            parseGoodsListDataFromServer(responseData);
            break;
        case TYPE_REQUEST_PIC: /* 商品图片请求 */
            savePictureDataToFile(responseData, requestID);
            break;
        case TYPE_UPLOAD_SOLD_INFO: /* 上报单次商品出售信息请求 */
            qDebug() << "Upload sold goods info success!";
            break;
        default:
            break;
        }
    }
}


/**************************************************************************
 * Function: initDatabase
 * Description: 初始化本地数据库。主要是检查本地数据库是否准备就绪。
 * Input:无
 * Output: 无
 * Return: 无
 *************************************************************************/
void CGoodsInfo::initDatabase()
{
    qDebug() << "init database...";

    QString strDatabasePath = CUtils::getFileFullPath(g_strDatabaseName);
    m_database = QSqlDatabase::addDatabase(g_strSqlDriverName, g_strConnectName);
    m_database.setDatabaseName(strDatabasePath);

    qDebug() << "Database path:" << strDatabasePath;
    if (!QFile::exists(strDatabasePath)) {
        qCritical() << "Cannot find the database file!";
        return;
    }

    m_bIsDBEmpty = false;

    if (!openDatabase()) {
        qDebug() << "Open database success!";
    } else {
        closeDatabase();
    }
    return;
}

/**************************************************************************
 * Function: openDatabase
 * Description: 打开本地数据库。用完之后需要关闭
 * Input:无
 * Output: 无
 * Return: 无
 *************************************************************************/
bool CGoodsInfo::openDatabase()
{
    if (m_bIsDBEmpty) {
        qCritical() << "Cannot open database, database is empty.";
        return false;
    }

    if (!m_database.open()) {
        qCritical() << "Open database failed!";
        if (m_database.isValid()) {
            m_database.removeDatabase(g_strConnectName);
        }
        return false;
    }

    qDebug() << "Open database success!";
    return true;
}

/**************************************************************************
 * Function: closeDatabase
 * Description: 关闭本地数据库
 * Input:无
 * Output: 无
 * Return: 无
 *************************************************************************/
void CGoodsInfo::closeDatabase()
{
    qDebug() << "Close database.";
    if (m_database.isOpen()) {
        m_database.close();
        /* Avoid run warning"connection is still in use" */
        QString name;
        {
            name = QSqlDatabase::database().connectionName();
        } /* QSqlDatabase::database() will be deleted */
        QSqlDatabase::removeDatabase(name);
    }
}

/**************************************************************************
 * Function: syncGoodsListFromServer
 * Description: 从服务器获取最新的商品信息列表，即本货机上最新发布的商品信息。一般是本货机
 *          上发布新商品之后，需要立即进行获取。
 * Input:无
 * Output: 无
 * Return: 无
 *************************************************************************/
void CGoodsInfo::syncGoodsListFromServer()
{
    /* HTTP请求的对象在整个程序中只实例化一次即可 */
    if (NULL == m_pHttpRequest) {
        m_pHttpRequest = new HttpRequest(this);
        connect(m_pHttpRequest, SIGNAL(dealHttpResponse(const QByteArray&, bool, const requestType_t, const QString &)),
                this, SLOT(slotHandleHttpRequestResult(const QByteArray&, bool, const requestType_t, const QString &)));
        m_pHttpRequest->init();
    }

    /** 请求数据为JSON数据，其形式为：
     * {"ops":9,"mac":"00:e0:61:4c:82:4c","message":[{"code":"0", "msg":"ok"}]}  */
    QJsonObject object;
    QJsonArray array;
    QJsonObject object1;
    object.insert("ops", 9);
    //QString mac = QNetworkInterface::allInterfaces()[0].hardwareAddress();
    QString mac = "00:00:00:00:00:00";
    object.insert("mac", mac);
    object1.insert("code", "0");
    object1.insert("msg", "OK");
    array.append(QJsonValue(object1));
    object.insert("message", QJsonValue(array));

    QJsonDocument document(object);
    qDebug() << "##request data:#" << document.toJson(QJsonDocument::Compact);

    m_pHttpRequest->startRequest("http://192.168.1.22:5555", document.toJson(), TYPE_REQUEST_GL, "0");
}


/**************************************************************************
 * Function: parseGoodsListDataFromServer
 * Description: 解析从服务器获取到的商品列表信息，并写入本地数据库
 * Input: 从服务器返回的JSON格式的商品列表信息，其格式为：
 *  {"ops":9, "data_hash":"4a2e6c550c19a054fa14054e8e1af5d8"
 *  ，"data":[{"id":1,"name":"可口可乐","picture":"http://bskl.jpg","price":4
 *  ,"introduce":"好喝到爆","type":0,"count":406,"coord":2},{"id":2,"name":"阿斯顿"
 *  ,"picture":"sad","price":89,"introduce":"很厉害","type":0,"count":500,"coord":0}
 *  ,{"id":3,"name":"特仑苏","picture":"asdas","price":100,"introduce":"不是每一种牛奶都叫特仑苏"
 *  ,"type":0,"count":3,"coord":12}],"error":{"code":"0","message":"get goods info success."}}
 * Output: 无
 * Return: 无
 *************************************************************************/
void CGoodsInfo::parseGoodsListDataFromServer(const QByteArray &jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isEmpty() || !document.isObject()) {
        qCritical() << "The goods list not a json object!";
        return;
    }
    QJsonObject objectAll = document.object();
    if (objectAll.isEmpty()) {
        qCritical() << "Json is empty!";
        return;
    }

    QJsonValue valueOps  = objectAll.value("ops");
    if (valueOps.isUndefined() || valueOps.toInt(0) != 9) {
        qCritical() << "Cannot find the key 'ops' or the key value is invalid.";
        return;
    }

    QJsonValue valueHash  = objectAll.value("data_hash");
    if (valueHash.isUndefined() || valueHash.toString("").isEmpty()) {
        qCritical() << "Cannot find the key 'data_hash' or the key value is empty.";
        return;
    }

    qDebug() << "Global goods list hash:" << g_strHashGoodslist;
    qDebug() << "New goods list hash:" << valueHash.toString("");


    /* 将从服务器获取到的商品列表hash值与本地数据库保存的hash进行比较，如果相同则表示本地商品信息不需要刷新 */
    if (g_strHashGoodslist.toLower() != valueHash.toString("").toLower()) {
        g_strHashGoodslist = valueHash.toString(""); /* Don't forget */
        QJsonValue valueData = objectAll.value("data");
        if (valueData.isUndefined() || !valueData.isArray() || valueData.toArray().isEmpty()) {
            qCritical() << "Cannot find the key 'data', or the key value is not a array or is empty!";
            return;
        }

        m_mutex.lock();

        m_mapGoods.clear();
        QJsonArray array = valueData.toArray();
        for (int i = 0; i < array.count(); ++i) {
            QJsonValue value = array.at(i);
            if (value.isObject()) {
                QJsonObject objectData = value.toObject();
                if (!objectData.isEmpty()) {
                    struGoodsInfo_t goodsInfo;
                    if (objectData.contains("id")) goodsInfo.id = objectData.value("id").toInt();
                    else qDebug() << "Cannot find key 'id'";
                    if (objectData.contains("name")) goodsInfo.strName = objectData.value("name").toString("");
                    else qDebug() << "Cannot find key 'name'";
                    if (objectData.contains("picture")) goodsInfo.strPictureUrl = objectData.value("picture").toString("");
                    else qDebug() << "Cannot find key 'picture'";
                    if (objectData.contains("price")) goodsInfo.nPrice = objectData.value("price").toInt();
                    else qDebug() << "Cannot find key 'price'";
                    if (objectData.contains("introduce")) goodsInfo.strIntroduce = objectData.value("introduce").toString("");
                    else qDebug() << "Cannot find key 'introduce'";
                    if (objectData.contains("type")) goodsInfo.type = objectData.value("type").toInt();
                    else qDebug() << "Cannot find key 'type'";
                    if (objectData.contains("count")) goodsInfo.nCount = objectData.value("count").toInt();
                    else qDebug() << "Cannot find key 'count'";
                    if (objectData.contains("coord")) goodsInfo.position = objectData.value("coord").toInt() - 1; /* local form 0 to 11 */
                    else qDebug() << "Cannot find key 'coord'";
                    m_mapGoods.insert(goodsInfo.position, goodsInfo);
                } else {
                    qWarning() << "The goods data is empty, index:" << i;
                }
            } else {
                qWarning() << "Find invalid data from goods list 'data'!";
            }
        }

        printGoodsList();
        refreshDBOfGoodsListInfo(g_strHashGoodslist);

        m_mutex.unlock();

        /* 根据从服务器获取到的新商品图片所在网络存放地址，下载图片到本地 */
        downloadPictureOfGoodsList();
    } else {
        qDebug() << "The hash of goods list is not change, don't need to sync local database.";
    }

}

/**************************************************************************
 * Function: refreshDBOfGoodsListInfo
 * Description: 根据从服务器获取到的新发布本柜商品信息，刷新本地数据库。只在本柜商品重新
 *  发布之后才进行刷新。
 * Input: 新商品信息的hash值。区分下一次向服务器请求之后是否需要刷新。
 * Output: 无
 * Return: 无
 *************************************************************************/
void CGoodsInfo::refreshDBOfGoodsListInfo(const QString &strHash)
{
    if (!openDatabase()) {
        return;
    }
    /* 先更新hash值，因为他们在不同的表 */
    refreshHashOfGoodsList(strHash);
    /* 清除之前的记录 */
    clearAllOfGoodsList();
    /* 插入最新的记录，注意这里是插入的方式，不是更新的方式 */
    refreshInfoOfGoodsList();

    closeDatabase();
}


/**************************************************************************
 * Function: refreshHashOfGoodsList
 * Description: 更新数据库中商品信息的hash值
 * Input: 最新商品信息的hash值
 * Output: 无
 * Return: 无
 *************************************************************************/
void CGoodsInfo::refreshHashOfGoodsList(const QString &strHash)
{
    QSqlQuery query(m_database);

    QString strExec = QString("SELECT hash_gl "
                              "FROM %1").arg(g_strVersionTableName);
    QString strTemp = "";

    /* 单向查询，所以调用setForwardOnly可以提高程序效率 */
    query.setForwardOnly(true);
    if (query.exec(strExec)) {
        qDebug() << "SQL query success: " << strExec;
        QSqlRecord record = query.record();
        int indexOfHash = record.indexOf("hash_gl");
        if (query.next()) {
            strTemp = query.value(indexOfHash).toString();
        } else {
            qWarning() << "Cann't find goods list hash value from 'hash_gl' table.";
        }
    } else {
        qDebug() << "SQL query failed: " << strExec;
        closeDatabase();
    }

    if (strTemp == strHash) {
        qDebug() << "hash_gl is not changed, return ...";
        return;
    }

    strExec = QString("UPDATE %1 SET hash_gl = '%2'").arg(g_strVersionTableName).arg(strHash);
    if (query.exec(strExec)) {
        qDebug() << "Update hash_gl success!";
    } else {
        qCritical() << "Update hash_gl failed!";
    }
}

/**************************************************************************
 * Function: clearAllOfGoodsList
 * Description: 清除当前本地数据库中所有商品信息记录，并删除对应保存在本地的商品图片
 * Input: 无
 * Output: 无
 * Return: 无
 *************************************************************************/
void CGoodsInfo::clearAllOfGoodsList()
{
    QSqlQuery query(m_database);

    QString strExec = QString("SELECT gid, path "
                              "FROM %1").arg(g_strGoodsInfoTableName);

    query.setForwardOnly(true);
    if (query.exec(strExec)) {
        qDebug() << "SQL success:" << strExec;
        QSqlRecord record;
        record = query.record();

        int indexID = record.indexOf("gid");
        int indexPath = record.indexOf("path");
        while (query.next()) {
            int gid = query.value(indexID).toInt();
            QString strPath = query.value(indexPath).toString();
            if (!QFile::remove(strPath)) {
                qWarning() << "Delete file failed: " << strPath;
            } else {
                qDebug() << "Delete file success: " << strPath;
            }

            QSqlQuery queryDel(m_database);
            QString strExecDel = QString("DELETE FROM %1 WHERE gid=%2")\
                    .arg(g_strGoodsInfoTableName).arg(gid);

            if (queryDel.exec(strExecDel)) {
                qDebug() << "SQL success: " << strExecDel;
            } else {
                qWarning() << "SQL failed: " << strExecDel;
            }
        }
    } else {
        qWarning() << "SQL failed:" << strExec;
    }
}

/**************************************************************************
 * Function: refreshInfoOfGoodsList
 * Description: 插入最新从服务器获取的商品信息。该商品信息保存在m_mapGoods中。
 * Input: 无
 * Output: 无
 * Return: 无
 *************************************************************************/
void CGoodsInfo::refreshInfoOfGoodsList()
{
    if (m_mapGoods.isEmpty()) {
        qWarning() << "The goods list is empty!";
        return ;
    }

#if 1
    QSqlQuery query(m_database);
    QString strExec = QString("INSERT INTO %1(gid, name, url, count, position, price, type, introduce) "
                              "VALUES(:gid, :name, :url, :count, :position, :price, :type, :introduce)")\
            .arg(g_strGoodsInfoTableName);

    if (query.prepare(strExec)) {

    } else {
        qDebug() << query.lastError().text();
        return ;
    }
    for (int i = 0; i < m_mapGoods.count(); ++i) {
        query.bindValue(":gid", m_mapGoods[i].id);
        query.bindValue(":name", m_mapGoods[i].strName);
        query.bindValue(":url", m_mapGoods[i].strPictureUrl);
        query.bindValue(":count", m_mapGoods[i].nCount);
        query.bindValue(":position", m_mapGoods[i].position);
        query.bindValue(":price", m_mapGoods[i].nPrice);
        query.bindValue(":type", m_mapGoods[i].type);
        query.bindValue(":introduce", m_mapGoods[i].strIntroduce);
        if (query.exec()) {
            qDebug() << "SQL success: " << strExec;
        } else {
            qCritical() << "SQL failed: " << strExec;
            qDebug() << query.lastError().text();
        }
    }
#else
    QSqlQuery query(m_database);
    QString strExec = "";

    for (int i = 0; i < m_mapGoods.count(); ++i) {
        strExec = QString("INSERT INTO %1(gid, name, url, count, position, price, type, introduce) "
                          "VALUES(%2, '%3', '%4', %5, %6, %7, %8, '%9')")\
                .arg(g_strGoodsInfoTableName).arg(m_mapGoods[i].id).arg(m_mapGoods[i].strName)\
                .arg(m_mapGoods[i].strPictureUrl).arg(m_mapGoods[i].nCount)\
                .arg(m_mapGoods[i].position).arg(m_mapGoods[i].nPrice)\
                .arg(m_mapGoods[i].type).arg(m_mapGoods[i].strIntroduce);
        if (query.exec(strExec)) {
            qDebug() << "SQL success: " << strExec;
        } else {
            qCritical() << "SQL failed: " << strExec;
        }
    }
#endif
}

/**************************************************************************
 * Function: downloadPictureOfGoodsList
 * Description: 调用HTTP请求，下载从服务器同步的最新商品的图片。
 *      该商品信息保存在m_mapGoods中。
 * Input: 无
 * Output: 无
 * Return: 无
 *************************************************************************/
void CGoodsInfo::downloadPictureOfGoodsList()
{
    if (NULL == m_pHttpRequest) {
        qCritical() << "Http Request object is empty!";
        return;
    }

    for (int i = 0; i < m_mapGoods.count(); ++i) {
        /* 请求ID使用当前时间ms，为了确保唯一，每次请求之后sleep 10ms */
        m_mapGoods[i].strRequestID = QString("").setNum(QDateTime::currentMSecsSinceEpoch());
        if (!m_mapGoods[i].strPictureUrl.isEmpty()) {
            m_pHttpRequest->startRequest(m_mapGoods[i].strPictureUrl, "", TYPE_REQUEST_PIC, m_mapGoods[i].strRequestID);
        }
        QThread::msleep(10);
    }
}

/**************************************************************************
 * Function: readGoodsListFromDB
 * Description: 从本地数据库同步商品列表信息
 * Input: 无
 * Output: 无
 * Return: 无
 *************************************************************************/
void CGoodsInfo::readGoodsListFromDB()
{
    if (!openDatabase()) {
        return;
    }

    QSqlQuery query(m_database);

    QString strExec = QString("SELECT hash_gl "
                              "FROM %1").arg(g_strVersionTableName);

    if (query.exec(strExec)) {
        qDebug() << "SQL query success: " << strExec;
        QSqlRecord record = query.record();
        int indexOfHash = record.indexOf("hash_gl");
        if (query.next()) {
            g_strHashGoodslist = query.value(indexOfHash).toString();
        } else {
            qWarning() << "Cann't find goods list hash value from 'hash_gl' table.";
        }
    } else {
        qDebug() << "SQL query failed: " << strExec;
        closeDatabase();
    }

    strExec = QString("SELECT gid, name, path, url, count, position, price, introduce "
                      "FROM %1").arg(g_strGoodsInfoTableName);
    query.setForwardOnly(true);
    if (query.exec(strExec)) {
        qDebug() << "SQL query success: " << strExec;
        QSqlRecord record = query.record();
        int indexOfGid = record.indexOf("gid");
        int indexOfName = record.indexOf("name");
        int indexOfPath = record.indexOf("path");
        int indexOfUrl = record.indexOf("url");
        int indexOfCount = record.indexOf("count");
        int indexOfPosition = record.indexOf("position");
        int indexOfPrice = record.indexOf("price");
        int indexOfIntroduce = record.indexOf("introduce");

        m_mapGoods.clear();

        while (query.next()) {
            struGoodsInfo_t struGoods;
            struGoods.clean();
            struGoods.id = query.value(indexOfGid).toInt();
            struGoods.strName = query.value(indexOfName).toString();
            struGoods.strPicturePath = query.value(indexOfPath).toString();
            struGoods.strPictureUrl = query.value(indexOfUrl).toString();
            struGoods.nCount = query.value(indexOfCount).toInt();
            struGoods.nPrice = query.value(indexOfPrice).toInt();
            struGoods.position = query.value(indexOfPosition).toInt();
            struGoods.strIntroduce = query.value(indexOfIntroduce).toString();

            m_mapGoods.insert(struGoods.position, struGoods);
        }

    } else {
        qDebug() << "SQL query failed: " << strExec;
        closeDatabase();
    }
}

/**************************************************************************
 * Function: savePictureDataToFile
 * Description: 保存从服务器获取的图片数据到本地
 * Input:
 *      byteArrayData: 图片数据
 *      strID: HTTP请求ID。通过ID来辨别该图片对应具体商品
 * Output: 无
 * Return: 无
 *************************************************************************/
void CGoodsInfo::savePictureDataToFile(const QByteArray &byteArrayData, const QString &strID)
{
    m_mutex.lock();

    for (int i = 0; i < m_mapGoods.count(); ++i) {
        if (m_mapGoods[i].strRequestID == strID) {
            QFileInfo fileInfo(m_mapGoods[i].strPictureUrl);
            m_mapGoods[i].strPicturePath = CUtils::getFileFullPath("share/" + strID) + "." + fileInfo.suffix();

            QFile file(m_mapGoods[i].strPicturePath);
            if (file.exists()) {
                file.remove();
            }
            if (!file.open(QIODevice::WriteOnly)) {
                qCritical() << "Open to write file failed: " << m_mapGoods[i].strPicturePath;
                break;
            } else {
                qDebug() << "file size: " << byteArrayData.length();
                if (file.write(byteArrayData)) {
                    qDebug() << "Save picture file success: " << m_mapGoods[i].strPicturePath;
                    /* 图片数据保存到本地之后，需要同步本地数据库中的商品图片本地路径信息 */
                    refreshDBOfPicturePathByOne(m_mapGoods[i]);
                } else {
                    qCritical() << "Save picture file failed: " << m_mapGoods[i].strPicturePath;
                }
                file.close();
            }
        }
    }

    m_mutex.unlock();
}

/**************************************************************************
 * Function: refreshDBOfPicturePathByOne
 * Description: 保存从服务器获取的图片数据到本地
 * Input: 单个商品信息。使用该参数，可以使知道要更新数据中具体商品记录。
 * Output: 无
 * Return: 无
 *************************************************************************/
void CGoodsInfo::refreshDBOfPicturePathByOne(struGoodsInfo_t &goodsInfo)
{
    if (!openDatabase()) {
        return;
    }

    QSqlQuery query(m_database);

    QString strExec = QString("UPDATE %1 SET path = '%2' WHERE gid = %3")\
            .arg(g_strGoodsInfoTableName).arg(goodsInfo.strPicturePath).arg(goodsInfo.id);

    if (query.exec(strExec)) {
        qDebug() << "SQL success:" << strExec;
    } else {
        qCritical() << "SQL failed: " << strExec;
    }

    closeDatabase();
}

/**************************************************************************
 * Function: printGoodsList
 * Description: 输出当前货架上的所有商品信息
 * Input: 无
 * Output: 无
 * Return: 无
 *************************************************************************/
void CGoodsInfo::printGoodsList()
{
    qDebug() << "#######################";
    qDebug() << "Goods list hash:" << g_strHashGoodslist;

    QMap<int, struGoodsInfo_t>::const_iterator iterator = m_mapGoods.constBegin();
    for (; iterator != m_mapGoods.constEnd(); ++iterator) {
        qDebug() << "id: " << (*iterator).id;
        qDebug() << "type: " << (*iterator).type;
        qDebug() << "name: " << (*iterator).strName;
        qDebug() << "path: " << (*iterator).strPicturePath;
        qDebug() << "picUrl: " << (*iterator).strPictureUrl;
        qDebug() << "count: " << (*iterator).nCount;
        qDebug() << "price: " << (*iterator).nPrice;
        qDebug() << "position: " << (*iterator).position;
        qDebug() << "introduce: " << (*iterator).strIntroduce;
    }
}

/**************************************************************************
 * Function: refreshSoldGoodsInfoToDB
 * Description: 根据当前出售信息，更新本地数据库中的商品信息；同时将当前出售信息上报给服务
 *      器，上报信息内容形式为：
 *      {"ops":5, "mac":"00:e0:61:4c:82:4c", "message":[{"code":"8"
 *      ,"msg":"10&5&1458716400"}, {"code":"1", "msg":"20&3&1458716400"}]}
 * Input: 无
 * Output: 无
 * Return: 成功返回true，失败返回false
 *************************************************************************/
bool CGoodsInfo::refreshSoldGoodsInfoToDB()
{
    if (!openDatabase()) {
        return false;
    }
    if (!m_struPayInfo.mapSelected.isEmpty()) {
        int leftCount = 0;
        int soldCount = 0;
        int singlePrice = 0;
        QSqlQuery query(m_database);
        QString strExec = "";

        QJsonObject objectUpload;
        objectUpload.insert("ops", 5);
        //        QString mac = QNetworkInterface::allInterfaces()[0].hardwareAddress();
        QString mac = "00:00:00:00:00:00";
        qDebug() << "" << mac;
        objectUpload.insert("mac", mac);
        QJsonArray jsonArray;
        uint currentTime = QDateTime::currentDateTime().toTime_t();

        for (int i = 0; i < m_struPayInfo.mapSelected.count(); ++i) {
            soldCount = m_struPayInfo.mapSelected[i];
            if (soldCount > 0) {
                QJsonObject objectChild;
                QMap<int, struGoodsInfo_t>::iterator it = m_mapGoods.begin();
                for (; it != m_mapGoods.end(); ++it) {
                    if (i == (*it).position) {
                        /* 这里顺便更新了缓存中的商品信息，以保持与数据库记录一致 */
                        (*it).nCount -= soldCount;
                        leftCount = (*it).nCount;
                        singlePrice = (*it).nPrice;
                        break;
                    }
                }
                strExec = QString("UPDATE %1 SET count=%2 WHERE position=%3")
                        .arg(g_strGoodsInfoTableName).arg(leftCount).arg(i);
                if (query.exec(strExec)) {

                } else {

                }
                objectChild.insert("code", QString("").setNum(i + 1));
                objectChild.insert("msg", QString("%1&%2&%3").arg(soldCount).arg(soldCount * singlePrice).arg(currentTime));
                jsonArray.append(objectChild);
            }
        }
        if (!jsonArray.isEmpty()) {
            objectUpload.insert("message", jsonArray);
            QJsonDocument document(objectUpload);
            qDebug() << "Upload sold message:" << document.toJson(QJsonDocument::Compact);
            m_pHttpRequest->startRequest("http://192.168.1.22:5555", document.toJson(), TYPE_UPLOAD_SOLD_INFO, "0");
        }

        m_struPayInfo.clear();
    }
    closeDatabase();
    return true;
}

/**************************************************************************
 * Function: setSelectedGoods
 * Description: 设置选购商品信息，由QML界面调用
 * Input:
 *      position: 所选购商品所在位置
 *      count: 购买数量
 *      start: 是否本次选购商品开始
 *      end: 是否本次选购商品结束。使用开始与结束，便于更新应该支付的总金额
 * Output: 无
 * Return: 成功返回true，失败返回false
 *************************************************************************/
bool CGoodsInfo::setSelectedGoods(int position, int count, bool start, bool end)
{
    if (start) {
        m_struPayInfo.clear();
    }

    m_mutex.lock();
    QMap<int, struGoodsInfo_t>::const_iterator it = m_mapGoods.constBegin();
    for (; it != m_mapGoods.constEnd(); ++it) {
        if (position == (*it).position) {
            m_struPayInfo.mapSelected.insert(position, count);
            m_struPayInfo.totalToPay += (*it).nPrice * count;
            break;
        }
    }
    m_mutex.unlock();


    if (end) {
        qDebug() << "Total to pay: " << m_struPayInfo.totalToPay;
        /* do something later ... */
    }
    return true;
}

/**************************************************************************
 * Function: setPayStyle
 * Description: 设置本次支付方式
 * Input: 支付方式
 * Output: 无
 * Return: 成功返回true，失败返回false
 *************************************************************************/
bool CGoodsInfo::setPayStyle(int style)
{
    qDebug() << "Set pay style: " << style;
    bool ret = true;
    switch (style) {
    case 0:
        m_struPayInfo.payStyle = TYPE_PAY_STYLE_COIN;
        break;
    case 1:
        m_struPayInfo.payStyle = TYPE_PAY_STYLE_CARD;
        break;
    default:
        ret = false;
        break;
    }

    return ret;
}

/**************************************************************************
 * Function: getTotalToPay
 * Description: 获取本次选购应当支付的总金额
 * Input: 无
 * Output: 无
 * Return: 返回本次选购应该支付的总金额，单位是元(￥)
 *************************************************************************/
int CGoodsInfo::getTotalToPay()
{
    return m_struPayInfo.totalToPay;
}

/**************************************************************************
 * Function: setPayResult
 * Description: 设置本次支付结果，或成功或失败或取消。
 * Input: 无
 * Output: 无
 * Return: 成功返回true，失败返回false
 *************************************************************************/
bool CGoodsInfo::setPayResult(bool bIsSuccess)
{
    return true;
}
