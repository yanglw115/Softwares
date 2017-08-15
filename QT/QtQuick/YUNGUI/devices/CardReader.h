#ifndef CCARDREADER_H
#define CCARDREADER_H

#include "SerialDevice.h"

#define READER_MAX_DATA_LEN           256
#define READER_MESSAGE_NON_BODY_LEN   5  //消息中除消息内容外的长度
#define READER_MSG_START              0x02  //消息起始字节
#define READER_MSG_EXIT               0x03  //消息停止字节
#define READER_REQ_M1                 0x34  //操作M1卡命令
#define READER_REQ_SEARCH             0x30  //寻M1卡子命令
#define READER_REQ_NUMBER             0x31  //获取M1卡序列号子命令
#define READER_REQ_VERIFY_KEYA        0x32  //验证M1卡Key A密码
#define READER_REQ_VERIFY_KEYB        0x39  //验证M1卡Key B密码
#define READER_REQ_READ_DATA          0x33  //读取M1卡数据
#define READER_REQ_WRITE_DATA         0x34  //写入M1卡数据
#define READER_REQ_UPDATE_KEY         0x35  //更新密码

#define READER_RES_SUCCESS            0x59  //操作成功
#define READER_RES_FAILD              0x4E  //操作失败
#define READER_RES_NO_CARD            0x45  //无卡
#define READER_RES_SECTOR_ERROR       0x31  //操作扇区号错误
#define READER_RES_KEY_ERROR          0x33  //验证密码错误
#define READER_RES_RW_BLOCK_ERROR     0x34  //读写数据块错误

#define BLOCK_DATA_LEN               16
#define KEY_DATA_LEN                 6
#define SERIAL_NUM_LEN               4

/*
struct CReaderMsgFmt {
    CReaderMsgFmt() {
        cStart = READER_MSG_START;
        cExit = READER_MSG_EXIT;
    }

    uchar cStart;  //消息起始字节
    uchar cLengthH; //通信数据段长度高位字节
    uchar cLengthL;  //通信数据段长度地位字节
    uchar cCmd;  //不同类型操作命令
    uchar cSubCmd;  //子操作命令
    uchar cDatas[READER_MAX_DATA_LEN]; //命令数据段
    uchar cExit;  //消息停止字节
    uchar cBCC;  //消息校验字节，从起始字节到停止字节取异或
};
*/

//读卡器处理类， 目前针对M1卡读写。
/*
    new->init->setSector->verifyKey->readBlock/writeBlock
                              ^               |
                              |_______________|
*/
class CCardReader : public CSerialDevice
{
    Q_OBJECT

public:
    CCardReader(QObject *parent = 0);
    ~CCardReader();

public slots:
    void slotInit();

public:
    void init(const CSerialPortConf& conf);

    //返回当前卡片是否贴入读卡器
    bool isCardIn();

    //寻卡
    void searchCard();

    //获取M1卡序列号
    ulong getM1CardNumber();

    //设定当前操作扇区号
    void setSector(uint num);

    //校验扇区密码，keyType表示密码类型，32表示keyA，39表示keyB。扇区密码固定6个字节
    void verifyKey(uchar keyType = READER_REQ_VERIFY_KEYB, const uchar *strKey = NULL);

    //更新扇区密码，只能更新A类型密码
    void updateKey(const uchar *strKey);

    //从当前扇区读取指定块的数据，读取的数据存入data中，data数组固定长度16。
    //传入的rData为NULL时为异步读取方式，读取的块数据会通过信号sigReadBlockData发送出去
    void readBlock(uint num);

    //将data数组数据写入指定块
    void writeBlock(uint num, const uchar wData[]);

    void HandleStatus(unsigned char nStatus,unsigned char *pData, int nDataLen);
    void onRecvResponse(unsigned char *pMsg, int nLen);
private:
    //发送获取卡序列号的命令
    void getSerialNumber();
    unsigned encodeCmdMsg(unsigned char cmd, unsigned char *pData, int nDataLen);
    //封装操作指令
    unsigned encodeCmdMsg(unsigned char cmd, unsigned char subCmd, unsigned char *pData, int nDataLen);

signals:
    void sigCardIn(bool);
    //读取到块数据信号
    void sigReadBlockData(uchar*);

private:
    bool m_bCardIn;  //卡片是否贴入
    ulong m_serialNum; //卡片序列号
    uint m_curSector;  //当前操作的扇区号
    uchar m_readDatas[BLOCK_DATA_LEN]; //存放从某个扇区读取的块数据
};

#endif // CCARDREADER_H
