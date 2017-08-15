#include <QStringList>
#include <QTimer>

#include "NoteAcceptor.h"
#include "util/Configuration.h"

//crc表用于计算Crc检验字节
static const unsigned int CrcTable[8*32]= {
    0x0000,0x8005,0x800F,0x000A,0x801B,0x001E,0x0014,0x8011,
    0x8033,0x0036,0x003C,0x8039,0x0028,0x802D,0x8027,0x0022,
    0x8063,0x0066,0x006C,0x8069,0x0078,0x807D,0x8077,0x0072,
    0x0050,0x8055,0x805F,0x005A,0x804B,0x004E,0x0044,0x8041,
    0x80C3,0x00C6,0x00CC,0x80C9,0x00D8,0x80DD,0x80D7,0x00D2,
    0x00F0,0x80F5,0x80FF,0x00FA,0x80EB,0x00EE,0x00E4,0x80E1,
    0x00A0,0x80A5,0x80AF,0x00AA,0x80BB,0x00BE,0x00B4,0x80B1,
    0x8093,0x0096,0x009C,0x8099,0x0088,0x808D,0x8087,0x0082,
    0x8183,0x0186,0x018C,0x8189,0x0198,0x819D,0x8197,0x0192,
    0x01B0,0x81B5,0x81BF,0x01BA,0x81AB,0x01AE,0x01A4,0x81A1,
    0x01E0,0x81E5,0x81EF,0x01EA,0x81FB,0x01FE,0x01F4,0x81F1,
    0x81D3,0x01D6,0x01DC,0x81D9,0x01C8,0x81CD,0x81C7,0x01C2,
    0x0140,0x8145,0x814F,0x014A,0x815B,0x015E,0x0154,0x8151,
    0x8173,0x0176,0x017C,0x8179,0x0168,0x816D,0x8167,0x0162,
    0x8123,0x0126,0x012C,0x8129,0x0138,0x813D,0x8137,0x0132,
    0x0110,0x8115,0x811F,0x011A,0x810B,0x010E,0x0104,0x8101,
    0x8303,0x0306,0x030C,0x8309,0x0318,0x831D,0x8317,0x0312,
    0x0330,0x8335,0x833F,0x033A,0x832B,0x032E,0x0324,0x8321,
    0x0360,0x8365,0x836F,0x036A,0x837B,0x037E,0x0374,0x8371,
    0x8353,0x0356,0x035C,0x8359,0x0348,0x834D,0x8347,0x0342,
    0x03C0,0x83C5,0x83CF,0x03CA,0x83DB,0x03DE,0x03D4,0x83D1,
    0x83F3,0x03F6,0x03FC,0x83F9,0x03E8,0x83ED,0x83E7,0x03E2,
    0x83A3,0x03A6,0x03AC,0x83A9,0x03B8,0x83BD,0x83B7,0x03B2,
    0x0390,0x8395,0x839F,0x039A,0x838B,0x038E,0x0384,0x8381,
    0x0280,0x8285,0x828F,0x028A,0x829B,0x029E,0x0294,0x8291,
    0x82B3,0x02B6,0x02BC,0x82B9,0x02A8,0x82AD,0x82A7,0x02A2,
    0x82E3,0x02E6,0x02EC,0x82E9,0x02F8,0x82FD,0x82F7,0x02F2,
    0x02D0,0x82D5,0x82DF,0x02DA,0x82CB,0x02CE,0x02C4,0x82C1,
    0x8243,0x0246,0x024C,0x8249,0x0258,0x825D,0x8257,0x0252,
    0x0270,0x8275,0x827F,0x027A,0x826B,0x026E,0x0264,0x8261,
    0x0220,0x8225,0x822F,0x022A,0x823B,0x023E,0x0234,0x8231,
    0x8213,0x0216,0x021C,0x8219,0x0208,0x820D,0x8207,0x0202
};

//纸币面额数组
static const uint noteValue[7] = {0,1,5,10,20,50,100};


CMsgProcessThread::CMsgProcessThread(QObject *parent) : QThread(parent)
{
    //qDebug()<<"Create a thread to process msg,"<<currentThreadId();
    m_bStop = false;
}

void CMsgProcessThread::Stop()
{
    m_bStop = true;
}

void CMsgProcessThread::run()
{
    while(!m_bStop){
        emit sigWorkReady();
        //m_pAcceptor->processMsg();
        msleep(1000);
    }
    m_bStop = false;
}



CNoteAcceptor::CNoteAcceptor(QObject *parent)
    : CSerialDevice(parent)
{
    m_curSeq = SSP_MESSAGE_SEQB;
    m_nTimerId = 0;
}

CNoteAcceptor::~CNoteAcceptor()
{
    m_pThread->Stop();
    while(m_pThread->isRunning());
    delete m_pThread;
}

void CNoteAcceptor::slotInit()
{
    CConfiguration *config = new CConfiguration(":/config.xml");
    QString info = config->GetConfiguration("Devices","PaperMoney");
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

    m_pTimerPoll = new QTimer;
    m_pTimerPoll->setInterval(POLL_INTERVAL);
    connect(m_pTimerPoll, SIGNAL(timeout()), this, SLOT(Poll()));

    this->SetTimeout(100);
    this->init(conf);
    this->Sync();
    this->RequestSetUp();
    this->SetInhibits(INHIBITS_VALUE_ALL);
//    this->StartAccept();
}

//打开该串口设备
void CNoteAcceptor::init(const CSerialPortConf& conf)
{
    CSerialDevice::init(conf);

    //poll命令会多次发送，这里先把poll命令信息记录下来，避免每次发送都要计算
    int len = this->encodeCmdMsg(SSP_CTRL_POLL, NULL, 0);
    memcpy(m_pollMsgSeqA, m_gDataSend, len);
    len = this->encodeCmdMsg(SSP_CTRL_POLL, NULL, 0);
    memcpy(m_pollMsgSeqB, m_gDataSend, len);

    m_curSeq = SSP_MESSAGE_SEQB;

    m_pThread = new CMsgProcessThread();
    connect(m_pThread, SIGNAL(sigWorkReady()), this, SLOT(processMsg()));
    connect(m_pThread, SIGNAL(finished()), m_pThread, SLOT(deleteLater()));
    m_pThread->start();
}

void CNoteAcceptor::Reset()
{
    int len = this->encodeCmdMsg(SSP_CTRL_RESET, NULL, 0);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

void CNoteAcceptor::RequestSetUp()
{
    int len = this->encodeCmdMsg(SSP_CTRL_SETUP_REQ, NULL, 0);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

void CNoteAcceptor::SetInhibits(Inhibits bits)
{
    unsigned char data[2] = {bits, 0x00};
    int len = this->encodeCmdMsg(SSP_CTRL_SET_INHIBITS, data, 2);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

void CNoteAcceptor::SetEnabled(bool enable)
{
    int len = 0;

    if(enable)
    {
        len = this->encodeCmdMsg(SSP_CTRL_ENABLE, NULL, 0);
    }
    else
    {
        len = this->encodeCmdMsg(SSP_CTRL_DISABLE, NULL, 0);
    }

    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

//开始接收纸币
void CNoteAcceptor::StartAccept()
{
    SetEnabled();
    //开始之后开启定时器定时发送poll命令
//    m_nTimerId = this->startTimer(POLL_INTERVAL);
    m_pTimerPoll->start();
}

//停止接收纸币
void CNoteAcceptor::EndAccept()
{
    SetEnabled(false);
//    this->killTimer(m_nTimerId);
    m_pTimerPoll->stop();
}

void CNoteAcceptor::Reject()
{
    int len = this->encodeCmdMsg(SSP_CTRL_REJECT, NULL, 0);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

void CNoteAcceptor::HandleStatus(unsigned char nStatus,unsigned char *pData, int nDataLen)
{
    QString msg = "";
    qDebug("Handle status(0x%02x),,Data len(%d)", nStatus, nDataLen);
    switch(nStatus)
    {
    case SSP_NTF_OK:
        msg = "Execute cmd success.";
        if(nDataLen > 0)
        {
            switch(pData[0])
            {
            case SSP_NTF_DISABLED:
                msg = "Acceptor is disabled";
                break;
            case SSP_NTF_CASHBOX_REMOVED:
                msg = "Cash Box is removed";
                break;
            case SSP_NTF_CASHBOX_REPLACED:
                msg = "Cash Box is replaced";
                break;
            case SSP_NTF_FRAUD_ATTEMPT:
                msg = "Have fraud attempt";
                break;
            case SSP_NTF_STACKER_FULL:
                msg = "Cash Box is full";
                break;
            case SSP_NTF_UNSAFE_JAM:
                msg = "Unsafe Jam";
                break;
            case SSP_NTF_SAFE_JAM:
                msg = "Safe Jam";
                break;
            case SSP_NTF_READ:
                msg = QString("Read a note in channel %1").arg(pData[1]);
                break;
            case SSP_NTF_STACKING:
                msg = "Note is stacking";
                if(nDataLen > 1 && pData[1] == SSP_NTF_STACKED)
                    msg = "Note is stacked";
                break;
            case SSP_NTF_REJECTING:
                msg = "Acceptor is rejecting notes";
                break;
            case SSP_NTF_REJECTED:
                msg = "Reject note finish";
                emit sigNoteRejected();
                break;
            case SSP_NTF_CREDIT:
            {
                uint val = noteValue[pData[1]];
                msg = QString("Accepted %1 Yuan.").arg(val);
                emit sigNoteAccepted(val);
            }
                break;
            default:
                msg += "Datas:";
                for(int i = 0; i < nDataLen; i++)
                    msg += "0x" + QString::number(pData[i], 16) + " ";
                break;
            }
        }
        break;
    case SSP_NTF_RESETTING:
        msg = "Acceptor is resetting";
        break;
    case SSP_NTF_WRONG_NUM_PARAMETER:
        msg = "Wrong Parameter";
        break;
    case SSP_NTF_CANNT_PROCESS:
        msg = "Command can not process";
        break;
    case SSP_NTF_CHECKSUM_ERR:
        msg = "Checksum error";
        break;
    case SSP_NTF_UNKNOWN_CMD:
        //msg = "Unknow command";
        //break;
    default:
        msg = "Unknow command";
        break;
    }

    qDebug("Receive message: %s", msg.toStdString().c_str());
}

//串口接收数据处理回调函数
void CNoteAcceptor::onRecvResponse(unsigned char *pMsg, int nLen)
{
    if(pMsg == NULL)
    {
        qFatal("Invalied argument");
        return;
    }

    int i;

    qDebug("onRecvResponse,lock");
    m_mutex.lock();
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
    m_mutex.unlock();
    qDebug("onRecvResponse,unlock");
}

void CNoteAcceptor::processMsg()
{
    int i;
//    qDebug("processMsg,lock");
    m_mutex.lock();
    if(!m_gDataRecv.isEmpty())
    {
        //数组中保存的字节长度小于3时不处理
        if(m_gDataRecv.size() < 3)
        {
            m_mutex.unlock();
            return;
        }

        //数组中保存的字节长度小于一段响应命令长度（m_gDataRecv[2]）时不作处理，等待下次读取串口数据后凑足一段命令
        if(m_gDataRecv.size() < m_gDataRecv[2] + SSP_MESSAGE_NON_BODY_LEN)
        {
            m_mutex.unlock();
            return;
        }

        //状态码
        unsigned char status = m_gDataRecv[3];
        //数据段
        unsigned char data[SSP_MESSAGE_MAX_BODY_LEN] = {0x00};
        //响应命令总长度
        int msgLen = m_gDataRecv[2] + SSP_MESSAGE_NON_BODY_LEN;
        //数据段长度
        int dataLen = m_gDataRecv[2] - 1;

        for(i = 0; i < dataLen; i++)
        {
            data[i] = m_gDataRecv[4+i];
        }

        //删除该段命令
        for(i = 0; i < msgLen; i++)
        {
            m_gDataRecv.removeAt(0);
        }


        //处理不同的状态码
        this->HandleStatus(status, data, dataLen);
    }
    m_mutex.unlock();
//    qDebug("processMsg,unlock");
}

//同步设备，查看设备是否连接
void CNoteAcceptor::Sync()
{
    int len = this->encodeCmdMsg(SSP_CTRL_SYNC, NULL, 0);
    if(m_pPortControl->SendData(m_gDataSend, len, m_timeout) < 0)
    {
        qDebug("Send cmd error!");
    }
}

//保持与设备通讯连接的poll命令
void CNoteAcceptor::Poll()
{
    int len = 0;

    if(m_curSeq == SSP_MESSAGE_SEQB)
    {
        len = m_pPortControl->SendData(m_pollMsgSeqA, SSP_MESSAGE_NON_BODY_LEN+1, m_timeout);
        m_curSeq = SSP_MESSAGE_SEQA;
    }
    else if(m_curSeq == SSP_MESSAGE_SEQA)
    {
        len = m_pPortControl->SendData(m_pollMsgSeqB, SSP_MESSAGE_NON_BODY_LEN+1, m_timeout);
        m_curSeq = SSP_MESSAGE_SEQB;
    }

    if(len < 0)
    {
        qDebug("Send cmd error!");
    }

}

void CNoteAcceptor::ResetCrc()
{
    m_crcl = 0xFF;
    m_crch = 0xFF;
}

void CNoteAcceptor::UpdateCrc(unsigned char num)
{
    unsigned int table_addr;
    table_addr = num ^ m_crch;
    m_crch = (CrcTable[table_addr] >> 8) ^ m_crcl;
    m_crcl = CrcTable[table_addr] & 0x00FF;
}

unsigned CNoteAcceptor::encodeCmdMsg(unsigned char cmd, unsigned char *pData, int nDataLen)
{
    int i;
    //放置从SEQ到数据段的字节数据，用于计算crcl和crch
    unsigned char tmp[SSP_MESSAGE_MAX_BODY_LEN] = {0x00};

    memset(m_gDataSend, 0x00, ACCPT_MAX_DATA_LEN);

    m_gDataSend[0] = SSP_MESSAGE_START; //起始位
    //SEQ字节位，两次请求命令标志字节交互使用0x80和0x00
    m_curSeq = (m_curSeq == SSP_MESSAGE_SEQB) ? SSP_MESSAGE_SEQA : SSP_MESSAGE_SEQB;
    tmp[0] = m_gDataSend[1] = m_curSeq;
    //Length字节位,命令字节和数据段的长度
    tmp[1] = m_gDataSend[2] = 1 + nDataLen;
    //命令字节
    tmp[2] = m_gDataSend[3] = cmd;
    //数据段指针位置
    unsigned char *ptr = &m_gDataSend[SSP_MESSAGE_BODY_SHIFT+1];
    for(i = 0; i < nDataLen; i++)
    {
        //将数据段内容填充到m_gDataSend和temp中
        tmp[3+i] = pData[i];
        *ptr = pData[i];
        //当数据段中包含0x7f字节时需要在这个位置以0x7f,0x7f代替，但是计算crc则不需要
        if(pData[i] == SSP_MESSAGE_START)
        {
            *(++ptr) = pData[i];
            m_gDataSend[2] += 1;
        }

        ptr++;
    }

    //计算crc
    ResetCrc();
    for(i = 0; i < 3+nDataLen; i++)
    {
        UpdateCrc(tmp[i]);
    }

    *(ptr++) = m_crcl;
    *(ptr++) = m_crch;

    return SSP_MESSAGE_NON_BODY_LEN + m_gDataSend[2];
}


void CNoteAcceptor::timerEvent(QTimerEvent *event)
{
    this->Poll();
}
