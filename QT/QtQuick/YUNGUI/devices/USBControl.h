#ifndef CUSBCONTROL_H
#define CUSBCONTROL_H

#include <QObject>

#include <libusb.h>

class CUSBControl : public QObject
{
    Q_OBJECT
public:
    explicit CUSBControl(QObject *parent = 0);
    ~CUSBControl();

    int openDevice(int nVendorId, int nProductId);

    void closeDevice();

    int sendData(uchar *pData, uint nDataLen, uint timeout = 0);

    int recvData(uchar *pData, uint nMaxLen);

private:
    void init();
    void unInit();
    bool findDeviceExist(int nVendorId, int nProductId);

signals:

public slots:

private:
    uint8_t m_nEndpointAddr;
    libusb_device_handle *m_pDevice; //a device handle
    libusb_context *m_pContext; //a libusb session
    libusb_device **m_devs;
};

#endif // CUSBCONTROL_H
