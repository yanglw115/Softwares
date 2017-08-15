#include <QStringList>

#include "CoinAcceptor.h"
#include "util/Configuration.h"


CCoinAcceptor::CCoinAcceptor(QObject *parent)
    : CSerialDevice(parent)
{
    m_bEnable = false;
}

CCoinAcceptor::~CCoinAcceptor()
{

}

void CCoinAcceptor::slotInit()
{
    CConfiguration *config = new CConfiguration(":/config.xml");
    QString info = config->GetConfiguration("Devices","Coin");
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

    this->SetTimeout(100);
    this->init(conf);

}

void CCoinAcceptor::init(const CSerialPortConf& conf)
{
    CSerialDevice::init(conf);
    //m_pPortControl->Flush();
    this->QueryStatus();
}

//查询硬币接收器的状态
void CCoinAcceptor::QueryStatus()
{
    int len = this->encodeCmdMsg(COIN_ACCPT_CMD_QUERY, NULL, 0);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

void CCoinAcceptor::SetEnabled(bool enable)
{
    int len = 0;
    if(enable)
    {
        len = this->encodeCmdMsg(COIN_ACCPT_CMD_ENABLE, NULL, 0);
    }
    else {
        len = this->encodeCmdMsg(COIN_ACCPT_CMD_DISABLE, NULL, 0);
        m_bEnable = false;
    }

    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

bool CCoinAcceptor::isEnabled()
{
    return m_bEnable;
}

void CCoinAcceptor::HandleStatus(unsigned char nStatus,unsigned char *pData, int nDataLen)
{
    QString msg = "";
    switch(nStatus)
    {
    case COIN_ACCPT_NTF_ACK:
        msg = "Receive ACK";
        break;
    case COIN_ACCPT_NTF_NAK:
        msg = "Receive NAK";
        break;
    case COIN_ACCPT_NTF_IDLE:
        msg = "Aceptor is idling";
        m_bEnable = true;
        break;
    case COIN_ACCPT_NTF_ACCEPTING:
        msg = "Accept coin";
        if(nDataLen > 0)
        {
            if(pData[0] == 0x01 || pData[0] == 0x02)
            {
                msg = "Accept 0.5 yuan";
            }
            else if(pData[0] == 0x03)
            {
                msg = "Accept 1.0 yuan";
                emit sigAcceptCoin();
            }
        }
        break;
    case COIN_ACCPT_NTF_DISABLED:
        msg = "Acceptor is disabled";
        m_bEnable = false;
        break;
    case COIN_ACCPT_NTF_SENSOR_ERROR:
        break;
    case COIN_ACCPT_NTF_FRAUD:
        break;
    case COIN_ACCPT_NTF_CHECKSUM_ERROR:
        msg = "The checksum of request is error";
        break;
    default:
        msg = QString("Unkonw message %1").arg(nStatus);
        break;
    }

    qDebug("MSG: %s", msg.toStdString().c_str());
}

bool CCoinAcceptor::IsTimeout(void)
{
    return false;
}

//将一次操作命令封装指定字节数组
unsigned CCoinAcceptor::encodeCmdMsg(unsigned char cmd, unsigned char *pData, int nDataLen)
{
    struct CCoinAcceptorMsgFmt msg;
    msg.cCmd = cmd;
    msg.cLen = 0x05;
    msg.cCheckSum = msg.cSync + msg.cLen + msg.cCmd + msg.cExit;

    memset(m_gDataSend, 0x00, ACCPT_MAX_DATA_LEN);

    m_gDataSend[0] = msg.cSync;
    m_gDataSend[1] = msg.cLen;
    m_gDataSend[2] = msg.cCmd;
    m_gDataSend[3] = msg.cExit;
    m_gDataSend[4] = msg.cCheckSum;

    return COIN_ACCPT_MIN_MSG_LEN;
}

void CCoinAcceptor::onRecvResponse(unsigned char *pMsg, int nLen)
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
        qDebug("data[%d]: 0x%x", i,m_gDataRecv[i]);
    }

    while(!m_gDataRecv.isEmpty())
    {
        //数组中保存的字节长度为1时不处理
        if(m_gDataRecv.size() < 2)
            break;

        //数组中保存的字节长度小于一段响应命令长度（m_gDataRecv[1]）时不作处理，等待下次读取串口数据后凑足一段命令
        if(m_gDataRecv.size() < m_gDataRecv[1])
            break;

        unsigned char status = 0x00;
        unsigned char data[COIN_ACCPT_MAX_DATA_LEN-COIN_ACCPT_MIN_MSG_LEN] = {0x00};
        int msgLen = m_gDataRecv[1];
        int dataLen = 0;

        //获取响应状态码
        status = m_gDataRecv[2];
        //一段响应命令中数据段的长度
        dataLen = msgLen - COIN_ACCPT_MIN_MSG_LEN;
        //提取数据段
        if(dataLen > 0)
            memcpy(data, pMsg+3, dataLen);

        //删除该段命令
        for(i = 0; i < msgLen; i++)
        {
            m_gDataRecv.removeAt(0);
        }


        //处理不同的状态码
        this->HandleStatus(status, data, dataLen);

    }
}
