#ifndef COINPAYOUT_H
#define COINPAYOUT_H

#include "SerialDevice.h"

#define COIN_PAYOUT_MSG_CMD_SHIFT           3
#define COIN_PAYOUT_BUF_LEN                 512
#define COIN_PAYOUT_STATUS_OK               0
#define COIN_PAYOUT_MSG_LEN                 6

#define COIN_PAYOUT_MSG_START               0x05
#define COIN_PAYOUT_MSG_FMT_ERR             0x20
#define COIN_PAYOUT_SLEEP_TIME              40*1000
#define COIN_PAYOUT_COINS_WITHOUT_NTY       0x10
#define COIN_PAYOUT_QUERY_STATUS            0x11
#define COIN_PAYOUT_RESET                   0x12
#define COIN_PAYOUT_QUERY_NUM               0x13
#define COIN_PAYOUT_COINS_WITH_NTF          0x14
#define COIN_PAYOUT_NTF_STATUS              0x04
#define COIN_PAYOUT_NTF_ONE_OUT             0x07
#define COIN_PAYOUT_NTF_SUCCEED             0x08
#define COIN_PAYOUT_NTF_ACK                 0xaa
#define COIN_PAYOUT_NTF_CLR_RECEIVED        0x09
#define COIN_PAYOUT_NTF_NAK                 0xbb

struct CCoinPayOutMsg
{
    CCoinPayOutMsg()
    {
        cHeader = 0x05;
        cDirection = 0x10;
        cMachineNo = 0x03;
    }

    unsigned char cHeader; //命令消息头
    unsigned char cDirection; //命令数据流向
    unsigned char cMachineNo; //机器号
    unsigned char cCmdStatus; //命令码
    unsigned char cData; //数据段
    unsigned char cCheckSum; //校验位
};


class CCoinPayout : public CSerialDevice
{
    Q_OBJECT
public:
    CCoinPayout(QObject *parent = 0);
    ~CCoinPayout();

public slots:
    void slotInitPayout();

public:
    //打开该串口设备
    void init(const CSerialPortConf& conf);

    void QueryStatus();

    void QueryRemain();

    //指定机器向用户支付n枚硬币
    void PayoutCoins(int n);

    bool isPayoutDone();

    void Reset(void);

    void HandleStatus(unsigned char nStatus,unsigned char *pData, int nDataLen);

    bool IsTimeout(void);

    //串口接收数据处理回调函数
    void onRecvResponse(unsigned char *pMsg, int nLen);
private:
    unsigned encodeCmdMsg(unsigned char cmd, unsigned char *pData, int nDataLen);

private:
    bool m_bPayoutFinish;

};

#endif
