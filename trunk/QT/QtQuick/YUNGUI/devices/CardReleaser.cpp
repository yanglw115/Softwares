#include "util/Configuration.h"

#include "CardReleaser.h"

CCardReleaser::CCardReleaser(QObject *parent)
    : CSerialDevice(parent)
{
    m_curSector = 0;
    m_bReadBlockDone = false;
    memset(m_readDatas, 0x00, BLOCK_DATA_LEN);
}

CCardReleaser::~CCardReleaser()
{

}

void CCardReleaser::slotInit()
{
    CConfiguration *config = new CConfiguration(":/config.xml");
    QString info = config->GetConfiguration("Devices","SellCard");
    QStringList lst = info.split(":");
    if(lst.count() < 6) {
        return;
    }
    CSerialPortConf conf;
    conf.bEnableParity = (lst[3] == "1")? true: false;
    if(conf.bEnableParity) {
        conf.bIsParityOdd = true;
    }
    conf.nBaudRate =  lst[2].toInt();
    conf.nStopBits =  lst[4].toInt();
    conf.nByteLen =  lst[5].toInt();
    conf.stPort = lst[0];

    this->init(conf);
}

//打开该串口设备
void CCardReleaser::init(const CSerialPortConf& conf)
{
    CSerialDevice::init(conf);

    this->reset();
}

void CCardReleaser::reset(ResetMode mode)
{
    uint len = this->encodeCmdMsg(ISSUE_REQ_RESET, mode, NULL, 0);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

//移动卡片到前端持卡位
void CCardReleaser::moveToFront()
{
    uint len = this->encodeCmdMsg(ISSUE_REQ_CARD_MOVE, MM_RETURN_TO_FRONT, NULL, 0);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

//移动卡片到射频卡读写位
void CCardReleaser::moveToRFPos()
{
    uint len = this->encodeCmdMsg(ISSUE_REQ_CARD_MOVE, MM_RETURN_TO_RF_POS, NULL, 0);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

//移动卡片到IC卡读写位
void CCardReleaser::moveToICPos()
{
    uint len = this->encodeCmdMsg(ISSUE_REQ_CARD_MOVE, MM_RETURN_TO_IC_POS, NULL, 0);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

//从前端弹出卡片
void CCardReleaser::popCard()
{
    uint len = this->encodeCmdMsg(ISSUE_REQ_CARD_MOVE, MM_EJECT_TO_FRONT, NULL, 0);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

//回收卡片
void CCardReleaser::recycleCard()
{
    uint len = this->encodeCmdMsg(ISSUE_REQ_CARD_MOVE, MM_CAPTURE_TO_BOX, NULL, 0);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

//设定是否允许从前端进卡
void CCardReleaser::setEnableInCard(bool enable)
{
    uchar subCmd = (enable ? ENABLE_IN_CARD : DISABLE_IN_CARD);
    uint len = this->encodeCmdMsg(ISSUE_REQ_CARD_INPUT, subCmd, NULL, 0);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

//寻找射频卡
void CCardReleaser::searchRFCard()
{
    uint len = this->encodeCmdMsg(ISSUE_REQ_CARD_CHECKOUT, RF_CARD, NULL, 0);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

//射频卡复位
void CCardReleaser::resetRFCard()
{
    uchar data[2] = {0x41,0x42};
    uint len = this->encodeCmdMsg(ISSUE_REQ_CARD_RW, RW_RESET, data, 2);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

//射频卡下电
void CCardReleaser::unloadRFCard()
{
    uint len = this->encodeCmdMsg(ISSUE_REQ_CARD_RW, NULL, 0);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

//设定当前操作扇区号
void CCardReleaser::setSector(uint num)
{
    m_curSector = num;
}

void CCardReleaser::verifyKey(uchar keyType, const uchar *strKey)
{
    uchar key[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
    uchar data[11] = {0x00,0x20,keyType,m_curSector,0x06};
    if(strKey != NULL)
    {
        memcpy(key, strKey, 6);
    }

    memcpy(&data[5], key, 6);

    uint len = this->encodeCmdMsg(ISSUE_REQ_CARD_RW, RW_VALUE_OP, data, 11);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

void CCardReleaser::updateKey(const uchar *strKey)
{
    uchar key[6] = {0x00};
    uchar data[11] = {0x00,0xd5,0x00,m_curSector,0x06};
    if(strKey != NULL)
    {
        memcpy(key, strKey, 6);
    }

    memcpy(&data[5], key, 6);

    uint len = this->encodeCmdMsg(ISSUE_REQ_CARD_RW, RW_VALUE_OP, data, 11);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

void CCardReleaser::readBlock(uint num, uchar rData[])
{
    uchar data[5] = {0x00,0xb0,m_curSector,num,0x01};
    uint len = this->encodeCmdMsg(ISSUE_REQ_CARD_RW, RW_VALUE_OP, data, 5);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
        return;
    }
    m_bReadBlockDone = false;

    if(rData != NULL)
    {
        while(true)
        {
            if(m_bReadBlockDone)
            {
                memcpy(rData, m_readDatas, BLOCK_DATA_LEN);
                break;
            }
        }
    }
}

void CCardReleaser::writeBlock(uint num, const uchar wData[])
{
    uchar data[5+BLOCK_DATA_LEN] = {0x00,0xd1,m_curSector,num,0x01};

    memcpy(&data[5], wData, BLOCK_DATA_LEN);

    uint len = this->encodeCmdMsg(ISSUE_REQ_CARD_RW, RW_VALUE_OP, data, 5+BLOCK_DATA_LEN);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
        return;
    }
}

//处理设备返回的状态，以及对应的数据段
void CCardReleaser::HandleStatus(unsigned char nStatus,unsigned char *pData, int nDataLen)
{
    QString msg = "";
    qDebug("Handle status(0x%02x),,Data len(%d)", nStatus, nDataLen);
    switch(nStatus)
    {
    case ISSUE_REQ_RESET:
        msg = "Reset response";
        break;
    case ISSUE_REQ_CARD_MOVE:
        msg = "Move card response";
        break;
    case ISSUE_REQ_CARD_INPUT:
        msg = "Ready rw card";
        break;
    case ISSUE_REQ_CARD_CHECKOUT:
        msg = "Check Card response";
        if(pData[0] == RF_CARD)
        {
            if(pData[1] == 0x32 && pData[2] == 0x32)
            {
                msg = "Check RF Card";
                emit sigCardInRFPos(true);
            }
            else
            {
                msg = "Can not find RF Card";
                emit sigCardInRFPos(false);
            }
        }
        break;
    case ISSUE_REQ_CARD_RW:
        msg = "Read and write card response";
        if(nDataLen > BLOCK_DATA_LEN
                && pData[nDataLen - 2] == 0x90
                && pData[nDataLen - 1] == 0x00)
        {
            memset(m_readDatas, 0x00, BLOCK_DATA_LEN);
            memcpy(m_readDatas, pData+4, BLOCK_DATA_LEN);
            m_bReadBlockDone = true;
            emit sigReadBlockData(m_readDatas);
        }
        break;
    default:
        msg = "Unkonw status";
        break;
    }

    msg += ",Datas:";
    for(int i = 0; i < nDataLen; i++)
        msg += "0x" + QString::number(pData[i], 16) + " ";

    qDebug("Receive message: %s", msg.toStdString().c_str());
}

bool CCardReleaser::IsTimeout(void)
{
    return false;
}

//串口接收数据处理回调函数
void CCardReleaser::onRecvResponse(unsigned char *pMsg, int nLen)
{
    if(pMsg == NULL)
    {
        qFatal("Invalied argument");
        return;
    }

    int i;

    //将传入数据添加到m_gDataRecv
    for(i = 0; i < nLen; i++)
    {
        m_gDataRecv.append(pMsg[i]);
    }

    qDebug("A Message");
    for(i = 0; i < m_gDataRecv.size(); i++)
    {
        qDebug("data[%d]: 0x%02x", i, m_gDataRecv[i]);
    }

    while(!m_gDataRecv.isEmpty())
    {
        if(m_gDataRecv.size() > 2 && m_gDataRecv[0] == 0x06 && m_gDataRecv[1] == 0xF2)
            m_gDataRecv.removeAt(0);
        //数组中保存的字节长度<4时不处理
        if(m_gDataRecv.size() < 4)
            break;

        int reqLen = (m_gDataRecv[2] << 8) | m_gDataRecv[3];
        //数组中保存的字节长度小于一段响应命令长度（m_gDataRecv[1]）时不作处理，等待下次读取串口数据后凑足一段命令
        if(m_gDataRecv.size() < reqLen + ISSUE_MESSAGE_NON_BODY_LEN)
            break;

        uchar status = 0x00;
        uchar *data = new uchar[reqLen-1];
        int msgLen = reqLen + ISSUE_MESSAGE_NON_BODY_LEN;
        int dataLen = 0;

        //获取响应状态码
        status = m_gDataRecv[5];
        //一段响应命令中数据段的长度
        dataLen = reqLen - 2;
        //提取数据段
        for(i = 0; i < dataLen; i++)
        {
            data[i] = m_gDataRecv[i+6];
        }

        //删除该段命令
        for(i = 0; i < msgLen; i++)
        {
            m_gDataRecv.removeAt(0);
        }


        //处理不同的状态码
        this->HandleStatus(status, data, dataLen);
        delete data;

    }
}

//封装操作指令
unsigned CCardReleaser::encodeCmdMsg(unsigned char cmd, unsigned char *pData, int nDataLen)
{
    return encodeCmdMsg(cmd, 0xFF, pData, nDataLen);
}

//封装操作指令
unsigned CCardReleaser::encodeCmdMsg(unsigned char cmd, unsigned char subCmd, unsigned char *pData, int nDataLen)
{
    uchar *ptr = &m_gDataSend[0];
    *(ptr++) = ISSUE_MSG_START;
    *(ptr++) = ISSUE_MSG_ADDRESS;
    ushort reqLen = 2;
    if(subCmd != 0xFF)
        reqLen++;
    reqLen += nDataLen;

    *(ptr++) = reqLen >> 8;
    *(ptr++) = reqLen & 0xFF;

    *(ptr++) = ISSUE_REQ_CMD_HEAD;
    *(ptr++) = cmd;
    if(subCmd != 0xFF)
    {
        *(ptr++) = subCmd;
    }
    if(nDataLen > 0)
    {
        memcpy(ptr, pData, nDataLen);
        ptr += nDataLen;
    }

    *(ptr++) = ISSUE_MSG_EXIT;

    uint msgLen = reqLen + ISSUE_MESSAGE_NON_BODY_LEN;
    uchar bcc = m_gDataSend[0];
    for(int i = 1; i < msgLen-1; i++)
    {
        bcc ^= m_gDataSend[i];
    }
    m_gDataSend[msgLen-1] = bcc;

    return msgLen;
}
