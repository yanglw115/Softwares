#include "CardReader.h"
#include "util/Configuration.h"

//设备通用反馈信息， 当卡放入读卡器后，读卡器会不停的发送此信息
static const uchar normalRes[11] = {0x02,0x00,0x06,0x31,0x44,0x30,0x34,0x32,0x30,0x03,0x74};

CCardReader::CCardReader(QObject *parent)
    : CSerialDevice(parent)
{
    m_bCardIn = false;
    m_serialNum = 0;
    m_curSector = 0;
    memset(m_readDatas, 0x00, BLOCK_DATA_LEN);
}

CCardReader::~CCardReader()
{

}

void CCardReader::slotInit()
{
    CConfiguration *config = new CConfiguration(":/config.xml");
    QString info = config->GetConfiguration("Devices","RfidCard");
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
    conf.stPort = lst[1];

    this->init(conf);
}

void CCardReader::init(const CSerialPortConf& conf)
{
    CSerialDevice::init(conf);

    //To Do Sth.
}

//返回当前卡片是否贴入读卡器
bool CCardReader::isCardIn()
{
    return m_bCardIn;
}

//寻卡
void CCardReader::searchCard()
{
    uint len = this->encodeCmdMsg(READER_REQ_M1, READER_REQ_SEARCH, NULL, 0);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

ulong CCardReader::getM1CardNumber()
{
    return m_serialNum;
}

//设定当前操作扇区号
void CCardReader::setSector(uint num)
{
    this->m_curSector = num;
}

//校验扇区密码，keyType表示密码类型，32表示keyA，39表示keyB。扇区密码固定6个字节
void CCardReader::verifyKey(uchar keyType, const uchar *strKey)
{
    uchar key[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
    uchar data[7] = {m_curSector};
    if(strKey != NULL)
    {
        memcpy(key, strKey, 6);
    }

    memcpy(&data[1], key, 6);

    uint len = this->encodeCmdMsg(READER_REQ_M1, keyType, data, 7);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

//更新扇区密码，只能更新A类型密码
void CCardReader::updateKey(const uchar *strKey)
{
    uchar key[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
    uchar data[7] = {m_curSector};
    if(strKey != NULL)
    {
        memcpy(key, strKey, 6);
    }

    memcpy(&data[1], key, 6);

    uint len = this->encodeCmdMsg(READER_REQ_M1, READER_REQ_UPDATE_KEY, data, 7);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

//从当前扇区读取指定块的数据，读取的数据存入data中，data数组固定长度16。
//传入的rData为NULL时为异步读取方式，读取的块数据会通过信号sigReadBlockData发送出去
void CCardReader::readBlock(uint num)
{
    uchar data[2] = {m_curSector, num};
    uint len = this->encodeCmdMsg(READER_REQ_M1, READER_REQ_READ_DATA, data, 2);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
        return;
    }
}

//将data数组数据写入指定块
void CCardReader::writeBlock(uint num, const uchar wData[])
{
    uchar data[2+BLOCK_DATA_LEN] = {m_curSector, num};

    memcpy(&data[2], wData, BLOCK_DATA_LEN);

    uint len = this->encodeCmdMsg(READER_REQ_M1, READER_REQ_WRITE_DATA, data, 2+BLOCK_DATA_LEN);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
        return;
    }
}

void CCardReader::HandleStatus(unsigned char nStatus,unsigned char *pData, int nDataLen)
{
    QString msg = "";
    if(nStatus == READER_REQ_M1)
    {
        switch(pData[0])
        {
        case READER_REQ_SEARCH:
            switch(pData[1])
            {
            case READER_RES_SUCCESS:
                msg = "Search RF card success";
                if(!m_bCardIn)
                {
                    m_bCardIn = true;
                    emit sigCardIn(true);
                    this->getSerialNumber();
                }
                break;
            case READER_RES_FAILD:
                msg = "Search RF card faild";
                m_bCardIn = false;
                break;
            case READER_RES_NO_CARD:
                msg = "No card in Reader";
                m_bCardIn = false;
                break;
            default:
                msg = "Operation statue code 0x" + QString::number(pData[1], 16);
                break;
            }

            break;
        case READER_REQ_NUMBER:
            //TO Do
            switch(pData[1])
            {
            case READER_RES_SUCCESS:
                {
                    msg = "Get Serial Number success";

                    //memset(m_readDatas, 0x00, BLOCK_DATA_LEN);
                    //memcpy(m_readDatas, pData+2, SERIAL_NUM_LEN);
                    m_serialNum = (pData[5] << 24)
                                    | (pData[4] << 16)
                                    | (pData[3] << 8)
                                    | pData[2];
                }
                break;
            case READER_RES_FAILD:
                msg = "Get Serial Number faild";
                break;
            case READER_RES_NO_CARD:
                msg = "No card in Reader";
                m_bCardIn = false;
                break;
            }

            break;
        case READER_REQ_VERIFY_KEYA:
        case READER_REQ_VERIFY_KEYB:
            switch(pData[2])
            {
            case READER_RES_SUCCESS:
                msg = "Verify Key success";
                break;
            case READER_RES_KEY_ERROR:
                msg = "Key Error";
                break;
            case READER_RES_NO_CARD:
                msg = "No card in Reader";
                m_bCardIn = false;
                break;
            default:
                msg = "Operation statue code 0x" + QString::number(pData[2], 16);
                break;
            }

            break;
        case READER_REQ_UPDATE_KEY:
            switch(pData[2])
            {
            case READER_RES_SUCCESS:
                msg = "Updata Key success";
                break;
            case READER_RES_SECTOR_ERROR:
                msg = "The Sector number is not verified";
                break;
            case READER_RES_KEY_ERROR:
                msg = "Key Error";
                break;
            case READER_RES_NO_CARD:
                msg = "No card in Reader";
                m_bCardIn = false;
                break;
            default:
                msg = "Operation statue code 0x" + QString::number(pData[2], 16);
                break;
            }

            break;
        case READER_REQ_READ_DATA:
            switch(pData[3])
            {
            case READER_RES_SUCCESS:
                {
                    msg = "Read block data success";

                    memset(m_readDatas, 0x00, BLOCK_DATA_LEN);
                    memcpy(m_readDatas, pData+4, BLOCK_DATA_LEN);
                    emit sigReadBlockData(m_readDatas);
                }
                break;
            case READER_RES_SECTOR_ERROR:
                msg = "The Sector number is not verified";
                break;
            case READER_RES_KEY_ERROR:
                msg = "Key Error";
                break;
            case READER_RES_RW_BLOCK_ERROR:
                msg = "Read block data error";
                break;
            case READER_RES_NO_CARD:
                msg = "No card in Reader";
                m_bCardIn = false;
                break;
            default:
                msg = "Operation statue code 0x" + QString::number(pData[3], 16);
                break;
            }

            break;
        case READER_REQ_WRITE_DATA:
            switch(pData[3])
            {
            case READER_RES_SUCCESS:
                msg = "Write block data success";
                break;
            case READER_RES_SECTOR_ERROR:
                msg = "The Sector number is not verified";
                break;
            case READER_RES_KEY_ERROR:
                msg = "Key Error";
                break;
            case READER_RES_RW_BLOCK_ERROR:
                msg = "Write block data error";
                break;
            case READER_RES_NO_CARD:
                msg = "No card in Reader";
                m_bCardIn = false;
                break;
            default:
                msg = "Operation statue code 0x" + QString::number(pData[3], 16);
                break;
            }

            break;
        default:
            msg += "Datas:";
            for(int i = 0; i < nDataLen; i++)
                msg += "0x" + QString::number(pData[i], 16) + " ";
            break;
        }
    }

    if(!m_bCardIn)
        emit sigCardIn(false);

    qDebug("Receive message: %s", msg.toStdString().c_str());
}

static bool s_skip_normal_resonse(QVector<uchar> &msgArray, const uchar normal[], int nLen)
{
    if(msgArray.size() < nLen)
    {
        return false;
    }

    int i = 0;
    for(i = 0; i < nLen; i++)
    {
        if(msgArray[i] != normal[i])
        {
            break;
        }
    }

    if(i == nLen)
    {
        for(int j = 0; j < nLen; j++)
        {
            msgArray.removeAt(0);
        }

        return true;
    }

    return false;
}

void CCardReader::onRecvResponse(unsigned char *pMsg, int nLen)
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
        //当接收到设备自动发来的通用数据时寻找卡片
        if(s_skip_normal_resonse(m_gDataRecv, normalRes, 11) && !m_bCardIn)
        {
            this->searchCard();
        }

        //数组中保存的字节长度<3时不处理
        if(m_gDataRecv.size() < 3)
            break;

        int reqLen = (m_gDataRecv[1] << 8) | m_gDataRecv[2];
        //数组中保存的字节长度小于一段响应命令长度（m_gDataRecv[1]）时不作处理，等待下次读取串口数据后凑足一段命令
        if(m_gDataRecv.size() < reqLen + READER_MESSAGE_NON_BODY_LEN)
            break;

        uchar status = 0x00;
        uchar *data = new uchar[reqLen-1];
        int msgLen = reqLen + READER_MESSAGE_NON_BODY_LEN;
        int dataLen = 0;

        //获取响应命令码
        status = m_gDataRecv[3];
        //一段响应命令中数据段的长度
        dataLen = reqLen - 1;
        //提取数据段
        for(i = 0; i < dataLen; i++)
        {
            data[i] = m_gDataRecv[i+4];
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

void CCardReader::getSerialNumber()
{
    uint len = this->encodeCmdMsg(READER_REQ_M1, READER_REQ_NUMBER, NULL, 0);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

unsigned CCardReader::encodeCmdMsg(unsigned char cmd, unsigned char *pData, int nDataLen)
{
    return encodeCmdMsg(cmd, 0xFF, pData, nDataLen);
}

unsigned CCardReader::encodeCmdMsg(unsigned char cmd, unsigned char subCmd, unsigned char *pData, int nDataLen)
{
    uchar *ptr = &m_gDataSend[0];

    *(ptr++) = READER_MSG_START;
    ushort reqLen = 2 + nDataLen;
    *(ptr++) = reqLen >> 8;
    *(ptr++) = reqLen & 0xFF;
    *(ptr++) = cmd;
    *(ptr++) = subCmd;
    if(nDataLen > 0)
    {
        memcpy(ptr, pData, nDataLen);
        ptr += nDataLen;
    }

    *(ptr++) = READER_MSG_EXIT;

    uint msgLen = reqLen + READER_MESSAGE_NON_BODY_LEN;
    uchar bcc = m_gDataSend[0];
    for(int i = 1; i < msgLen-1; i++)
    {
        bcc ^= m_gDataSend[i];
    }
    m_gDataSend[msgLen-1] = bcc;

    return msgLen;
}
