#include "SerialDevice.h"

CSerialDevice::CSerialDevice(QObject *parent)
    : QObject(parent)
{
    memset(m_gDataSend, 0x00, ACCPT_MAX_DATA_LEN);

    m_pPortControl = new CSerialPortControl(this);
    m_pPortControl->SetRecvMsgCb(this);
}

CSerialDevice::~CSerialDevice()
{
    m_pPortControl->ClosePort();
    delete m_pPortControl;
}

//打开该串口设备
void CSerialDevice::init(const CSerialPortConf& conf)
{
    if(m_pPortControl->OpenAndConfPort(conf) != 0)
    {
        qDebug("Open device error");
        return;
    } else {
        qDebug("Open device success!");
    }
}

void CSerialDevice::close()
{
    m_pPortControl->ClosePort();
}

void CSerialDevice::HandleStatus(unsigned char nStatus,unsigned char *pData, int nDataLen)
{

}

void CSerialDevice::SetTimeout(int mesecs)
{
    m_timeout = mesecs;
}

unsigned CSerialDevice::encodeCmdMsg(unsigned char cmd, unsigned char *pData, int nDataLen)
{
    return 0;
}

//串口接收数据处理回调函数
void CSerialDevice::onRecvResponse(unsigned char *pMsg, int nLen)
{

}
