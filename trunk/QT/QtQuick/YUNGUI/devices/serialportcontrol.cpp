#include "serialportcontrol.h"
#include "SerialDevice.h"

#include <QtDebug>

CSerialPortControl::CSerialPortControl(QObject *parent)
    : QObject(parent)
    , m_serial(NULL)
    , m_pCCb(NULL)
{
    m_serial = new QSerialPort(this);
}

CSerialPortControl::~CSerialPortControl()
{

}

void CSerialPortControl::SetRecvMsgCb(void *classCb)
{
    this->m_pCCb = classCb;
}

int CSerialPortControl::OpenAndConfPort(const CSerialPortConf& conf)
{
    if(ConfigurePort(conf) != 0)
    {
        qDebug("Config port error!");
        return -1;
    }
    if (!m_serial->open(QIODevice::ReadWrite))
    {
        qDebug() << "Can't open " <<  conf.stPort << ", error code: " << m_serial->error();
        return -2;
    }

    connect(m_serial, SIGNAL(readyRead()), SLOT(slotRecvMsg()));

    return 0;
}

int CSerialPortControl::OpenSerialPort(const QString port)
{
    CSerialPortConf conf;
    conf.stPort = port;

    return OpenAndConfPort(conf);
}

int CSerialPortControl::ConfigurePort(const CSerialPortConf& conf)
{
    if(conf.stPort.isEmpty())
    {
        qDebug("Invalite config data!");
        return -1;
    }
    m_serial->setPortName(conf.stPort);
    m_serial->setBaudRate(conf.nBaudRate);
    m_serial->setStopBits((QSerialPort::StopBits)conf.nStopBits);
    if(conf.bIsParityOdd)
        m_serial->setParity(QSerialPort::OddParity);
    m_serial->setReadBufferSize(conf.nByteLen);

    return 0;
}

int CSerialPortControl::GetSerialPort(void)
{
    return 0;
}

void CSerialPortControl::ClosePort(void)
{
    if(m_serial->isOpen())
    {
        m_serial->flush();
        m_serial->close();
    }
}

void CSerialPortControl::Flush(void)
{
   m_serial->flush();
}

int CSerialPortControl::SendData(const unsigned char* pMessage,int nLen, int timeout)
{
    int wLen = 0;


    qDebug("in SendData,A Request Message");
    for(int i = 0; i < nLen; i++)
    {
        qDebug("data[%d]: 0x%x", i,pMessage[i]);
    }

    wLen = m_serial->write((char*)pMessage, nLen);

    if (!m_serial->waitForBytesWritten(timeout))
    {
        return -1;
    }

    return wLen;
}

int  CSerialPortControl::ReceiveData(unsigned char* pBuffer, int nBufferLen, int timeout)
{
    if (m_serial->waitForReadyRead(timeout))
    {
        QByteArray responseData = m_serial->readAll();
        if(responseData.size() > nBufferLen)
        {
            qDebug("Invalited buffer length");
            return -1;
        }
        memcpy(pBuffer, responseData.data(), responseData.size());
        return responseData.size();
    }

    return 0;
}

void CSerialPortControl::slotRecvMsg()
{
    unsigned char buffer[ACCPT_MAX_DATA_LEN] = {0x00};
    QByteArray responseData = m_serial->readAll();

    if(responseData.size() > ACCPT_MAX_DATA_LEN)
    {
        qDebug("Invalited buffer length");
        return;
    }
    memcpy(buffer, (unsigned char*)responseData.data(), responseData.size());
    //for(int i = 0; i < responseData.size(); i++)
    //
    //    qDebug("data[%d]: 0x%x",i,buffer[i]);
    //}
    if(this->m_pCCb != NULL)
    {
        CSerialDevice *device = static_cast<CSerialDevice *>(this->m_pCCb);
        device->onRecvResponse(buffer, responseData.size());
    }
}
