#include <QStringList>

#include "CoinPayout.h"
#include "util/Configuration.h"

CCoinPayout::CCoinPayout(QObject *parent)
    : CSerialDevice(parent)
{

}

CCoinPayout::~CCoinPayout()
{

}

void CCoinPayout::slotInitPayout()
{
    CConfiguration *config = new CConfiguration(":/config.xml");
    QString info = config->GetConfiguration("Devices","ReturnCoin");
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

//打开该串口设备
void CCoinPayout::init(const CSerialPortConf& conf)
{
    CSerialDevice::init(conf);

    this->QueryStatus();
}

void CCoinPayout::QueryStatus()
{
    unsigned char data = 0x00;
    int len = this->encodeCmdMsg(COIN_PAYOUT_QUERY_STATUS, &data, 1);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

void CCoinPayout::QueryRemain()
{
    unsigned char data = 0x00;
    int len = this->encodeCmdMsg(COIN_PAYOUT_QUERY_NUM, &data, 1);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

//指定机器向用户支付n枚硬币
void CCoinPayout::PayoutCoins(int n)
{
    unsigned char data = n & 0xff;
    int len = this->encodeCmdMsg(COIN_PAYOUT_COINS_WITH_NTF, &data, 1);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }

    m_bPayoutFinish = false;
}

bool CCoinPayout::isPayoutDone()
{
    return m_bPayoutFinish;
}

void CCoinPayout::Reset(void)
{
    unsigned char data = 0x00;
    int len = this->encodeCmdMsg(COIN_PAYOUT_RESET, &data, 1);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

void CCoinPayout::HandleStatus(unsigned char nStatus,unsigned char *pData, int nDataLen)
{
    QString msg = "";
    switch(nStatus)
    {
    case COIN_PAYOUT_NTF_STATUS:
        {//处理返回状态
            if(pData[0] == 0x00)
            {
                msg = "All status is fine";
            }
            else
            {
                if(pData[0] & 0x01)
                    msg = "Motor problem";
                if((pData[0] >> 1) & 0x01)
                    msg = "Insufficient Coins";
                if((pData[0] >> 3) & 0x01)
                    msg = "Prism Sensor Failure";
                if((pData[0] >> 4) & 0x01)
                    msg = "Shaft Sensor Failure";
                if((pData[0] >> 5) & 0x01)
                    msg = "Dispenser is busy";
            }
        }
        break;
    case COIN_PAYOUT_NTF_ONE_OUT:
        msg = "Payout one coin";
        break;
    case COIN_PAYOUT_NTF_SUCCEED:
        msg = "Payout finish";
        m_bPayoutFinish = true;
        break;
    case COIN_PAYOUT_NTF_CLR_RECEIVED:
        msg = "Receive CLEAR";
        break;
    case COIN_PAYOUT_NTF_ACK:
        {
            msg = "Receive ACK";
            if(pData[0] > 0)
            {
                msg = QString("Receive ACK,and remain %1 coins").arg(pData[0]);
            }
        }
        break;
    case COIN_PAYOUT_NTF_NAK:
        msg = "Receive NAK";
        break;
    default:
        msg = "Unknow response status";
        break;
    }

    qDebug("Current status: %s", msg.toStdString().c_str());
}

bool CCoinPayout::IsTimeout(void)
{
    return false;
}

unsigned CCoinPayout::encodeCmdMsg(unsigned char cmd, unsigned char *pData, int nDataLen)
{
    struct CCoinPayOutMsg msg;
    msg.cCmdStatus = cmd;
    msg.cData = *pData;
    msg.cCheckSum = msg.cHeader + msg.cDirection + msg.cMachineNo + msg.cCmdStatus + msg.cData;

    memset(m_gDataSend, 0x00, ACCPT_MAX_DATA_LEN);
    m_gDataSend[0] = msg.cHeader;
    m_gDataSend[1] = msg.cDirection;
    m_gDataSend[2] = msg.cMachineNo;
    m_gDataSend[3] = msg.cCmdStatus;
    m_gDataSend[4] = msg.cData;
    m_gDataSend[5] = msg.cCheckSum;

    /*
    qDebug("A Request Message");
    for(int i = 0; i < 10; i++)
    {
        qDebug("data[%d]: 0x%x", i,m_gDataSend[i]);
    }
    */
    return COIN_PAYOUT_MSG_LEN;
}

//串口接收数据处理回调函数
void CCoinPayout::onRecvResponse(unsigned char *pMsg, int nLen)
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
        if(m_gDataRecv.size() < COIN_PAYOUT_MSG_LEN)
            break;

        unsigned char status = m_gDataRecv[3];
        unsigned char data = m_gDataRecv[4];
        int msgLen = COIN_PAYOUT_MSG_LEN;

        //删除该段命令
        for(i = 0; i < msgLen; i++)
        {
            m_gDataRecv.removeAt(0);
        }


        //处理不同的状态码
        this->HandleStatus(status, &data, 1);

    }
}
