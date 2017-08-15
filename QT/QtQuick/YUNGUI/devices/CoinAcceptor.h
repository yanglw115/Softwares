#ifndef COINACCEPTOR_H
#define COINACCEPTOR_H

#include "SerialDevice.h"

#define COIN_ACCPT_MIN_MSG_LEN           5
#define COIN_ACCPT_CMD_SHIFT             2
#define COIN_ACCPT_MAX_DATA_LEN          512
#define COIN_ACCPT_MAX_MSG_LEN           (MAX_DATA_LEN+5)
#define COIN_ACCPT_MSGLEN_SENSOR_ERR     6
#define COIN_ACCPT_MSGLEN_NORMAL         5
#define COIN_ACCPT_MESSAGE_FORMAT_ERROR  -1

#define COIN_ACCPT_STATUS_OK              0x0
#define COIN_ACCPT_MSG_START              0x90
#define COIN_ACCPT_MSG_EXIT               0x03
#define COIN_ACCPT_CMD_ENABLE             0x01
#define COIN_ACCPT_CMD_DISABLE            0x02
#define COIN_ACCPT_CMD_GETVERSION         0x03
#define COIN_ACCPT_CMD_RESET              0x05
#define COIN_ACCPT_CMD_QUERY              0x11
#define COIN_ACCPT_NTF_ACK                0x50
#define COIN_ACCPT_NTF_NAK                0x4b
#define COIN_ACCPT_NTF_IDLE               0x11
#define COIN_ACCPT_NTF_ACCEPTING          0x12
#define COIN_ACCPT_NTF_DISABLED           0x14
#define COIN_ACCPT_NTF_SENSOR_ERROR       0x16
#define COIN_ACCPT_NTF_FRAUD              0x17
#define COIN_ACCPT_NTF_CHECKSUM_ERROR     0x18

//请求命令组成格式
struct CCoinAcceptorMsgFmt
{
    CCoinAcceptorMsgFmt()
    {
        //消息的第一个字节是固定的
        cSync = COIN_ACCPT_MSG_START;
        cExit = COIN_ACCPT_MSG_EXIT;
    }
    unsigned char cSync;
    unsigned char cLen;
    unsigned char cCmd;
    unsigned char cExit;
    unsigned char cCheckSum;
};

//投币器操作处理类， 调用流程new->init->SetEnabled
class CCoinAcceptor : public CSerialDevice
{
    Q_OBJECT
public:
    CCoinAcceptor(QObject *parent = 0);
    virtual ~CCoinAcceptor();

public slots:
    void slotInit();

    //打开该串口设备
    void init(const CSerialPortConf& conf);

    //查询硬币接收器的状态
    void QueryStatus();

    //设定硬币接收器是否可用， 一次接收操作就是先使接收器可用，当接收的回馈信息是接收完毕时再使其不可用
    void SetEnabled(bool enable = true);

    //硬币接收器是否可用
    bool isEnabled();

    //处理设备返回的状态，以及对应的数据段
    void HandleStatus(unsigned char nStatus,unsigned char *pData, int nDataLen);

    bool IsTimeout(void);
    
    //串口接收数据处理回调函数
    void onRecvResponse(unsigned char *pMsg, int nLen);

private:
    //封装操作指令
    unsigned encodeCmdMsg(unsigned char cmd, unsigned char *pData, int nDataLen);

signals:
    //接收到硬币信号
    void sigAcceptCoin();

private:
    bool m_bEnable;
};

#endif
