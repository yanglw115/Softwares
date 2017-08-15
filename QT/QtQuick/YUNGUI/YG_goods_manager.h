#ifndef CGoodsManager_H
#define CGoodsManager_H

#include <QObject>

#include "devices/CoinAcceptor.h"
#include "YG_goods_info.h"

class CGoodsInfo;

class CGoodsManager : public QObject
{
    Q_OBJECT
public:
    explicit CGoodsManager(QObject *parent = 0);
    ~CGoodsManager();
    /* 以Q_INVOKABLE宏定义开关的函数，是为了让商品管理类在QML中的实例可以直接调用 */
    Q_INVOKABLE int getGoodsListSize();
    Q_INVOKABLE bool bIsGoodsInfoExist(int position);
    Q_INVOKABLE bool bIsGoodsEmpty(int position);
    Q_INVOKABLE QString getGoodsName(int position);
    Q_INVOKABLE QString getGoodsPicPath(int position);
    Q_INVOKABLE QString getGoodsCount(int position);
    Q_INVOKABLE QString getGoodsPrice(int position);

    Q_INVOKABLE bool setSelectedGoods(int position, int count, bool start = false, bool end = false);
    Q_INVOKABLE bool setPayStyle(int style);
    Q_INVOKABLE bool startToPay();
    Q_INVOKABLE bool startOddChange(uint);
    Q_INVOKABLE bool stopToPay();
    Q_INVOKABLE bool refreshSoldGoodsInfoToDB();
    Q_INVOKABLE QString getTotalToPay();
    Q_INVOKABLE bool setPayResult(bool bIsSuccess);
    Q_INVOKABLE bool readValidMoneyOfCard();
    Q_INVOKABLE bool writeValidMoneyOfCard(double money);
    Q_INVOKABLE void checkCardInStatus();

signals:
    /* 当有硬币或纸币投入时，发射该信号给QML界面 */
    void sigGetMoney(unsigned int);
    void sigCardIn(bool);
    void sigGetCardMoney(double);

public slots:
    void slotHandleCoinIn();
    void slotHandleNoteIn(unsigned int);
    void slotHandleCardIn(bool);
    void slotHandleCardMoneyData(uchar *data);

private:
    CGoodsInfo *m_pGoodsInfo;
};

#endif // CGoodsManager_H
