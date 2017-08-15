#ifndef CGoodsInfo_H
#define CGoodsInfo_H

#include <QList>
#include <QMap>
#include <QMutex>
#include <QObject>
#include <QPoint>
#include <QSqlDatabase>

#include "devices/CardReader.h"
#include "devices/CoinAcceptor.h"
#include "devices/CoinPayout.h"
#include "devices/NoteAcceptor.h"
#include "devices/CardReleaser.h"
#include "http/httprequest.h"

#ifdef __cplusplus
class CGoodsInfo;
extern "C" {
    bool createGoodsInfoInstance(QObject *parent = NULL);
    CGoodsInfo *getGoodsInfoInstance();
}
#endif

enum enumPayStyle {
    TYPE_PAY_STYLE_COIN = 0,  /* 投币支付，可包括硬币与纸币同时进行 */
    TYPE_PAY_STYLE_CARD = 1   /* 刷卡支付 */
};

/* 已选购商品信息存入到QMap中，便于进行管理 */
typedef QMap<int, int> mapSelectedGoods_t; // <int position, int count>

/* 付款信息，包括所有选购商品、付费类型、应付费、支付和找零等信息 */
typedef struct struPayInfo {
    mapSelectedGoods_t mapSelected; /* 所有选购商品 */
    enumPayStyle payStyle; /* 付款方式 */
    int totalToPay; /* 应支付 */
    int inputMoney; /* 已支付 */
    int outputMoney; /* 找零 */

    void clear()
    {
        mapSelected.clear();
        payStyle = TYPE_PAY_STYLE_COIN;
        totalToPay = 0;
        inputMoney = 0;
        outputMoney = 0;
    }
} struPayInfo_t;

typedef struct struGoodsInfo {
    QString strName; /* 商品名称 */
    QString strPicturePath; /* 商品图片所在本地存储位置 */
    QString strPictureUrl; /* 商品图片所在网络存储位置 */
    QString strIntroduce; /* 商品描述信息 */
    QString strRequestID; /* 商品图片通过http请求时所用的请求ID */
    int id; /* 商品ID */
    int type; /* 商品类型 */
    int nCount; /* 剩余商品数量 */
    int nPrice; /* 商品单价 */
    int position; /* 商品在货架的位置，货架为2*6结构 server: 1 ~ 12, local: 0 ~ 11*/

    void clean()
    {
        strName.clear();
        strPicturePath.clear();
        strPictureUrl.clear();
        strRequestID.clear();
        id = 0;
        type = 0;
        nCount = 0;
        nPrice = 0.0;
        position = -1;
    }
} struGoodsInfo_t;

class CGoodsInfo: public QObject
{
    Q_OBJECT
public:
    explicit CGoodsInfo(QObject *parent = NULL);
    ~CGoodsInfo();

public slots:
    void slotStartInit();
    void slotHandleHttpRequestResult(const QByteArray &strResponseData, bool bRetSucc,
                                     const requestType_t type, const QString &requestID);

public:
    bool setSelectedGoods(int position, int count, bool start = false, bool end = false);
    bool setPayStyle(int style);
    int getTotalToPay();
    bool setPayResult(bool bIsSuccess);
    bool refreshSoldGoodsInfoToDB();

private:
    void initDatabase();
    bool openDatabase();
    void closeDatabase();
    void syncGoodsListFromServer();
    void parseGoodsListDataFromServer(const QByteArray &strJsonData);
    void refreshDBOfGoodsListInfo(const QString &strHash);
    void refreshHashOfGoodsList(const QString &strHash);
    void clearAllOfGoodsList();
    void refreshInfoOfGoodsList();
    void downloadPictureOfGoodsList();
    void readGoodsListFromDB();
    void savePictureDataToFile(const QByteArray &strData, const QString &ID);
    void refreshDBOfPicturePathByOne(struGoodsInfo_t &goodsInfo);
    void printGoodsList();

public:
    CCoinAcceptor *m_pCoinAcceptor; /* 硬币接收器 */
    CCoinPayout *m_pCoinPayout;     /* 硬币找零器 */
    CNoteAcceptor *m_pNoteAcceptor; /* 纸币接收器 */
    CCardReader *m_pCardReadr;      /* 读卡器 */
    CCardReleaser *m_pCardReleaser; /* 制卡器,当然也可读卡 */
    QSqlDatabase m_database;        /* sql数据库 */
    bool m_bIsDBEmpty;              /* 数据库内容是否为空 */
    QMutex m_mutex;
    QMap<int, struGoodsInfo_t> m_mapGoods; /* 当前货架商品信息 */
    struPayInfo_t m_struPayInfo;           /* 当前购物过程支付信息 */
    HttpRequest *m_pHttpRequest;           /* HTTP请求实现对象指针 */
};

#endif // CGoodsInfo_H
