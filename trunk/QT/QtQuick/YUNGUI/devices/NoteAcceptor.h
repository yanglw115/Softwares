#ifndef CNOTEACCEPTOR_H
#define CNOTEACCEPTOR_H

#include "SerialDevice.h"

#include <QThread>
#include <QMutex>

class QTimer;
#define VENDING_MACHINE_TIMEOUT      500
#define SSP_MESSAGE_MAX_BODY_LEN     255//消息内容最大长度
#define SSP_MESSAGE_LEN              512//整条消息最大长度
#define SSP_MESSAGE_BODY_SHIFT       3  //消息内容偏移
#define SSP_MESSAGE_NON_BODY_LEN     5  //消息中除消息内容外的长度
#define SSP_MESSAGE_START            0x7f//起始位
#define SSP_MESSAGE_SEQA             0x80//序列号A和B交替出现
#define SSP_MESSAGE_SEQB             0x00

#define SSP_NTF_NUM                  62//接收到的不同类型的命令数

#define POLL_INTERVAL                2000


//以下是纸币器的控制命令详情参考文档
#define SSP_CTRL_RESET               0x01
#define SSP_CTRL_SET_INHIBITS        0x02
#define SSP_CTRL_DISPLAY_ON          0x03
#define SSP_CTRL_DISPLAY_OFF         0x04
#define SSP_CTRL_SETUP_REQ           0x05
#define SSP_CTRL_POLL                0x07
#define SSP_CTRL_REJECT              0x08
#define SSP_CTRL_DISABLE             0x09
#define SSP_CTRL_ENABLE              0x0a
#define SSP_CTRL_SYNC                0x11
#define SSP_CTRL_REJECT_CAUSE             0x17
#define SSP_CTRL_HOLD                     0x18
#define SSP_CTRL_ENABLE_VERSION_EVENTS    0x19
#define SSP_CTRL_PAYOUT_AMOUNT            0x33
#define SSP_CTRL_SET_COIN_AMOUNT          0x34
#define SSP_CTRL_GET_COIN_AMOUNT          0x35
#define SSP_CTRL_SET_ROUTING              0x3b
#define SSP_CTRL_GET_ROUTING              0x3c
#define SSP_CTRL_GET_MIN_PAYOUT           0x3e
#define SSP_CTRL_EMPTY_ALL                0x3f
#define SSP_CTRL_GET_NOTE_POSITION        0x41
#define SSP_CTRL_PAYOUT_NOTE              0x42
#define SSP_CTRL_STACK_NOTE               0x43
#define SSP_CTRL_SET_GENERATOR            0x4a
#define SSP_CTRL_SET_MODULUS              0x4b
#define SSP_CTRL_REQUEST_KEY_EXCHANGE     0x4c
#define SSP_CTRL_GET_NOTE_COUNTERS        0x58
#define SSP_CTRL_RESET_NOTE_COUNTERS      0x59

//下面是接收到的来自纸币器的命令通知
#define SSP_NTF_CHANNEL_INFO         0x00
#define SSP_NTF_CHANNEL_DISABLE      0xb5
#define SSP_NTF_EMPTYING             0xc2
#define SSP_NTF_EMPTY                0xc3
#define SSP_NTF_TRANS2STACKER        0xc9
#define SSP_NTF_STACKING             0xcc
#define SSP_NTF_NOTE_IN_BAZEL_HOLD   0xce
#define SSP_NTF_DEVICE_FULL          0xcf
#define SSP_NTF_DISPENSED            0xd2
#define SSP_NTF_JAMMED               0xd5
#define SSP_NTF_HALTED               0xd6
#define SSP_NTF_DISPENSING           0xda
#define SSP_NTF_NOTE_IN_PAYOUT       0xdb
#define SSP_NTF_CASHBOX_REMOVED      0xe3
#define SSP_NTF_CASHBOX_REPLACED     0xe4
#define SSP_NTF_FRAUD_ATTEMPT        0xe6
#define SSP_NTF_STACKER_FULL         0xe7
#define SSP_NTF_DISABLED             0xe8
#define SSP_NTF_UNSAFE_JAM           0xe9
#define SSP_NTF_SAFE_JAM             0xea
#define SSP_NTF_STACKED              0xeb
#define SSP_NTF_REJECTED             0xec
#define SSP_NTF_REJECTING            0xed
#define SSP_NTF_CREDIT               0xee
#define SSP_NTF_READ                 0xef
#define SSP_NTF_OK                   0xf0
#define SSP_NTF_RESETTING            0xf1
#define SSP_NTF_UNKNOWN_CMD          0xf2
#define SSP_NTF_WRONG_NUM_PARAMETER  0xf3
#define SSP_NTF_CANNT_PROCESS        0xf5
#define SSP_NTF_SOFTWARE_ERR         0xf6
#define SSP_NTF_CHECKSUM_ERR         0xf7
#define SSP_NTF_FAILURE              0xf8
#define SSP_KEY_NOTE_NOT_SET         0xfa


//纸币面额对应通道位
enum Inhibits{
    INHIBITS_VALUE_1 = 0x01 | 0x80, //1元纸币对应的通道位
    INHIBITS_VALUE_5 = 0x04 | 0x80, //5元纸币
    INHIBITS_VALUE_10 = 0x08 | 0x80, //10元纸币
    INHIBITS_VALUE_20 = 0x10 | 0x80, //20元纸币
    INHIBITS_VALUE_50 = 0x20 | 0x80, //50元纸币
    INHIBITS_VALUE_100 = 0x40 | 0x80, //100元纸币
    INHIBITS_VALUE_ALL = INHIBITS_VALUE_1 | INHIBITS_VALUE_5
                        | INHIBITS_VALUE_10 | INHIBITS_VALUE_20
                        | INHIBITS_VALUE_50 | INHIBITS_VALUE_100
};

class CMsgProcessThread : public QThread
{
    Q_OBJECT
public:
    explicit CMsgProcessThread(QObject *parent = 0);
    void Stop();

protected:
    void run();

signals:
    void sigWorkReady();
private:
    volatile bool m_bStop;
};

//纸币接收器操作类， 简单调用流程 new->init->sync->StartAccept->EndAccept
class CNoteAcceptor : public CSerialDevice
{
    Q_OBJECT
public:
    CNoteAcceptor(QObject *parent = 0);
    ~CNoteAcceptor();

public slots:
    void slotInit();
    void Poll();

public:

    //打开该串口设备
    void init(const CSerialPortConf& conf);
    //充值接收器
    void Reset();
    //同步设备连接
    void Sync();
    //设备轮询命令，用于维持设备的通信
//    void Poll();
    //查询识币器通道配置情况
    void RequestSetUp();
    //设定对应面值的纸币通道打开或关闭
    void SetInhibits(Inhibits bits);
    //设定识币器是否可用
    void SetEnabled(bool enable = true);
    //开始纸币接收
    void StartAccept();
    //停止纸币接收
    void EndAccept();
    //拒绝接收纸币
    void Reject();
    //处理返回响应状态数据
    void HandleStatus(unsigned char nStatus,unsigned char *pData, int nDataLen);

    //串口接收数据处理回调函数，将msg存入字节数组缓冲
    void onRecvResponse(unsigned char *pMsg, int nLen);

protected:
    void timerEvent(QTimerEvent *event);
private:
    //充值Crc字节
    void ResetCrc();
    //根据传入的num计算Crc字节数据
    void UpdateCrc(unsigned char num);
    //封装请求命令
    unsigned encodeCmdMsg(unsigned char cmd, unsigned char *pData, int nDataLen);

signals:
    //接收到纸币信号
    void sigNoteAccepted(unsigned int);
    //纸币被拒绝信号
    void sigNoteRejected();
public slots:
    //处理接收数据数组中的数据
    void processMsg();

private:
    unsigned char m_curSeq; //记录当前命令的标志字节（0x00 or 0x80）
    unsigned char m_crcl; //crcl校验字节
    unsigned char m_crch; //crch校验字节
    unsigned char m_pollMsgSeqA[SSP_MESSAGE_NON_BODY_LEN+1]; //基于0x80标志字节的poll命令
    unsigned char m_pollMsgSeqB[SSP_MESSAGE_NON_BODY_LEN+1]; //基于0x00标志字节的poll命令
    CMsgProcessThread *m_pThread; //处理接收数据缓冲数组线程
    QMutex m_mutex;
    int m_nTimerId;
    QTimer *m_pTimerPoll;
};

#endif // CNOTEACCEPTOR_H
