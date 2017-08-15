#ifndef CCARDRELEASER_H
#define CCARDRELEASER_H

#include "SerialDevice.h"

#define ISSUE_MAX_DATA_LEN           512
#define ISSUE_MESSAGE_NON_BODY_LEN   6  //消息中除消息内容外的长度
#define ISSUE_MSG_START              0xF2  //消息起始字节
#define ISSUE_MSG_EXIT               0x03  //消息停止字节
#define ISSUE_MSG_ADDRESS            0x00  //机器出厂字节标志
#define ISSUE_REQ_CMD_HEAD           0x43  //请求命令起始字节
#define ISSUE_RES_CMD_HEAD           0x50  //响应命令起始字节
#define ISSUE_REQ_RESET              0x30  //复位操作命令
#define ISSUE_REQ_CARD_MOVE          0x32  //卡移动操作命令
#define ISSUE_REQ_CARD_INPUT         0x33  //卡接收操作命令
#define ISSUE_REQ_CARD_CHECKOUT      0x50  //检测卡片
#define ISSUE_REQ_CARD_RW            0x60  //卡读写操作命令

#define BLOCK_DATA_LEN               16
#define KEY_DATA_LEN                 6


//复位子操作
typedef enum _ResetMode
{
    RST_RETURN_TO_FRONT = 0x30 , //复位并移动卡到出卡口位
    RST_CAPTURE_TO_BOX = 0x31, //复位并回收卡
    RST_WITHOUT_MOVEMENT = 0x33 , //复位， 不移动卡
}ResetMode;

//卡片移动子操作
typedef enum _MoveCardMode
{
    MM_RETURN_TO_FRONT = 0x30, // 移动卡到前端持卡位
    MM_RETURN_TO_IC_POS = 0x31, // 移动卡到IC位
    MM_RETURN_TO_RF_POS = 0x32 ,// 移动卡到射频位
    MM_CAPTURE_TO_BOX = 0x33 , //回收卡
    MM_EJECT_TO_FRONT = 0x39 , //从前端弹出卡片
}MoveCardMode;

enum _CardInMode
{
    ENABLE_IN_CARD = 0x30,
    DISABLE_IN_CARD = 0x31,
};

enum _CardRWMode
{
    RW_RESET = 0x30,  //射频卡复位
    RW_UNLOAD = 0x31, //射频卡下电
    RW_QUERY_STATUS = 0x32, //查询射频卡状态
    RW_VALUE_OP = 0x33, //读写数据操作
};

enum _CardType
{
    IC_CARD = 0x30, //IC卡
    RF_CARD = 0x31, //射频卡
};

/*
struct CIssueMsgFmt
{
    CIssueMsgFmt(uchar cmd, uchar subCmd, uchar *pData, uint nDataLen)
    {
        cStart = ISSUE_MSG_START;
        cAddr = ISSUE_MSG_ADDRESS;
        cCmdHead = ISSUE_REQ_CMD_HEAD;
        cExit = ISSUE_MSG_EXIT;
        cCmd = cmd;
        cSubCmd = subCmd;
        memset(cDatas, 0x00, ISSUE_MAX_DATA_LEN);
        reqLen = 2;
        if(subCmd != 0xFF)
            reqLen++;
        if(nDataLen > 0)
        {
            memcpy(cDatas, pData, nDataLen);
            reqLen += nDataLen;
        }
        cLengthH = (reqLen & 0xff00) >> 8;
        cLengthL = reqLen & 0x00ff;
        msgLen = reqLen + ISSUE_MESSAGE_NON_BODY_LEN;
    }

    uint fillBuffer(uchar *pMsg)
    {
        uchar *ptr = pMsg;
        *(ptr++) = cStart;
        *(ptr++) = cAddr;
        *(ptr++) = cLengthH;
        *(ptr++) = cLengthL;
        *(ptr++) = cCmdHead;
        *(ptr++) = cCmd;
        uint dataLen = reqLen - 2;
        if(cSubCmd != 0xff)
        {
            *(ptr++) = cSubCmd;
            dataLen--;
        }
        if(dataLen > 0)
            memcpy(ptr, cDatas, dataLen);
        *(ptr++) = cExit;

        for(int i = 0;i < msgLen - 1;i++)
        {
             cBCC ^= pMsg[i];
        }
        pMsg[msgLen-1] = cBCC;
        return msgLen;
    }

    uchar cStart;
    uchar cAddr;
    uchar cLengthH;
    ushort reqLen;
    uchar cLengthL;
    uchar cCmdHead;
    uchar cCmd;
    uchar cSubCmd;
    uchar cDatas[ISSUE_MAX_DATA_LEN];
    uchar cExit;
    uchar cBCC;

    uint msgLen;
};
*/


//发卡机处理类，包含读写卡功能
class CCardReleaser : public CSerialDevice
{
    Q_OBJECT
public:
    CCardReleaser(QObject *parent = 0);
    ~CCardReleaser();

public slots:
    void slotInit();

public:
    //打开该串口设备
    void init(const CSerialPortConf& conf);

    //复位操作，包括不移动卡/移动卡到出口/回收卡三种模式
    void reset(ResetMode mode = RST_WITHOUT_MOVEMENT);

    //移动卡片到前端持卡位
    void moveToFront();

    //移动卡片到射频卡读写位
    void moveToRFPos();

    //移动卡片到IC卡读写位
    void moveToICPos();

    //从前端弹出卡片
    void popCard();

    //回收卡片
    void recycleCard();

    //设定是否允许从前端进卡
    void setEnableInCard(bool enable = true);

    //寻找射频卡
    void searchRFCard();

    //射频卡复位
    void resetRFCard();

    //射频卡下电
    void unloadRFCard();

    //设定当前操作扇区号
    void setSector(uint num);

    //校验扇区密码，keyType表示密码类型，00表示keyA，01表示keyB。扇区密码固定6个字节
    void verifyKey(uchar keyType = 0x01, const uchar *strKey = NULL);

    //更新扇区密码，只能更新A类型密码
    void updateKey(const uchar *strKey);

    //从当前扇区读取指定块的数据，读取的数据存入data中，data数组固定长度16。
    //传入的rData为NULL时为异步读取方式，读取的块数据会通过信号sigReadBlockData发送出去
    void readBlock(uint num, uchar rData[]);

    //将data数组数据写入指定块
    void writeBlock(uint num, const uchar wData[]);

    //处理设备返回的状态，以及对应的数据段
    void HandleStatus(unsigned char nStatus,unsigned char *pData, int nDataLen);

    bool IsTimeout(void);

    //串口接收数据处理回调函数
    void onRecvResponse(unsigned char *pMsg, int nLen);

private:
    //封装操作指令
    unsigned encodeCmdMsg(unsigned char cmd, unsigned char *pData, int nDataLen);
    //封装操作指令
    unsigned encodeCmdMsg(unsigned char cmd, unsigned char subCmd, unsigned char *pData, int nDataLen);

signals:
    void sigCardInRFPos(bool);
    void sigReadBlockData(uchar*);

private:
    uint m_curSector;  //当前操作的扇区号
    bool m_bReadBlockDone; //读取块数据成功
    uchar m_readDatas[BLOCK_DATA_LEN]; //存放从某个扇区读取的块数据
};

#endif // CCARDRELEASER_H
