#ifndef SERIALPORTCONTROL_H
#define SERIALPORTCONTROL_H

#include<QString>
#include <QtSerialPort/QSerialPort>

#define BAUDRATE  B9600
#define ACCPT_MAX_DATA_LEN          512

struct CSerialPortConf
{
    CSerialPortConf() :nStopBits(1),bEnableParity(false),bIsParityOdd(false),
    		nBaudRate(9600),nByteLen(8)
    {}

    int         nStopBits;
    bool        bEnableParity;
    bool        bIsParityOdd;
    int         nBaudRate;
    int         nByteLen;
    QString stPort;
};

typedef void (*RECVCB)(unsigned char *pMsg, int nLen, void* userData);

class CSerialPortControl : public QObject
{
    Q_OBJECT

public:
    CSerialPortControl(QObject *parent = 0);
    ~CSerialPortControl();

    void ClosePort(void);
    int OpenAndConfPort(const CSerialPortConf& conf);
    int ConfigurePort(const CSerialPortConf& conf);
    int OpenSerialPort(const QString port);
    int GetSerialPort(void);
    void Flush(void);
    int SendData(const unsigned char* pMessage,int nLen, int timeout = 100);
    int  ReceiveData(unsigned char* pBuffer, int nBufferLen, int timeout = 100);
    void SetRecvMsgCb(void *classCb);

private slots:
    void slotRecvMsg();

private:
    QSerialPort *m_serial;
    int            m_nComFd;
    void *m_pCCb;
};

#endif
