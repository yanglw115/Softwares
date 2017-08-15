#ifndef CSERIALDEVICE_H
#define CSERIALDEVICE_H

#include "serialportcontrol.h"
#include <QObject>
#include <QVector>

class CSerialDevice : public QObject
{
    Q_OBJECT
public:
    explicit CSerialDevice(QObject *parent = 0);
    virtual ~CSerialDevice();

    virtual void init(const CSerialPortConf& conf);
    virtual void close();
    virtual void HandleStatus(unsigned char nStatus,unsigned char *pData, int nDataLen);
    virtual void SetTimeout(int mesecs);
    virtual void onRecvResponse(unsigned char *pMsg, int nLen);
private:
    virtual unsigned encodeCmdMsg(unsigned char cmd, unsigned char *pData, int nDataLen);

signals:

public slots:

protected:
    unsigned char m_gDataSend[ACCPT_MAX_DATA_LEN];
    QVector<unsigned char> m_gDataRecv;
    CSerialPortControl *m_pPortControl;
    int m_timeout;
};

#endif // CSERIALDEVICE_H
