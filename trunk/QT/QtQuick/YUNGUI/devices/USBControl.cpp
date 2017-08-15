#include "USBControl.h"

CUSBControl::CUSBControl(QObject *parent) : QObject(parent)
{
    m_nEndpointAddr = 0;
    m_pDevice = NULL;
    m_pContext = NULL;
    m_devs = NULL;

    init();
}

CUSBControl::~CUSBControl()
{
    closeDevice();
    unInit();
}

int CUSBControl::openDevice(int nVendorId, int nProductId)
{
    if(findDeviceExist(nVendorId, nProductId))
    {
        m_pDevice = libusb_open_device_with_vid_pid(m_pContext, nVendorId, nProductId);
        if(m_pDevice == NULL)
        {
            qDebug("Open device error");
            return -1;
        }
        else
        {
            int res = 0;

            /*Check if kenel driver attached*/
            if(libusb_kernel_driver_active(m_pDevice, 0))
            {
              res = libusb_detach_kernel_driver(m_pDevice, 0); // detach driver
              if(res != 0)
              {
                  qDebug("Detach kernel driver error");
                  return res;
              }
            }
            res = libusb_claim_interface(m_pDevice, 0); //claim interface 0 (the first) of device (mine had jsut 1)
            if(res < 0)
            {
                qDebug("Cannot Claim Interface");
                return res;
            }
        }
    }

    return 0;
}

void CUSBControl::closeDevice()
{

    if(m_pDevice != NULL)
    {
        if(libusb_release_interface(m_pDevice, 0) != 0)
        {
            qDebug("Release interface error");
        }
        libusb_close(m_pDevice);
        m_pDevice = NULL;
    }
}

int CUSBControl::sendData(uchar *pData, uint nDataLen, uint timeout)
{
    if(pData == NULL || nDataLen == 0)
    {
        return -1;
    }

    qDebug("CUSBControl::sendData");
    for(int i = 0; i < nDataLen; i++)
    {
        qDebug("data[%d]: 0x%02x", i, pData[i]);
    }

    int len = 0;
    int res = 0;
    res = libusb_bulk_transfer(m_pDevice, m_nEndpointAddr, pData, nDataLen, &len, timeout);
    if(res != 0)
    {
        qDebug("Send data error(%d)", res);
        return res;
    }

    return len;
}

int CUSBControl::recvData(uchar *pData, uint nMaxLen)
{
    return 0;
}

void CUSBControl::init()
{
    int res = 0;
    ssize_t cnt;

    res = libusb_init(&m_pContext);
    if(res < 0)
    {
        qDebug("Init libusb error(%d)", res);
        return;
    }
    libusb_set_debug(m_pContext, 3);

    cnt = libusb_get_device_list(m_pContext, &m_devs);
    if(cnt < 0)
    {
       qDebug("Get usb device list error");
    }
}

void CUSBControl::unInit()
{
    if(m_devs != NULL)
    {
        libusb_free_device_list(m_devs, 1);
        m_devs = NULL;
    }

    libusb_exit(m_pContext);
    m_pContext = NULL;
}

bool CUSBControl::findDeviceExist(int nVendorId, int nProductId)
{
    libusb_device *dev;
    int i = 0;

    while ((dev = m_devs[i++]) != NULL)
    {
        struct libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0)
        {
            qDebug("failed to get device descriptor,(%d)", r);
            return false;
        }
        if(desc.idVendor == nVendorId && desc.idProduct == nProductId)
        {
            struct libusb_config_descriptor *config;
            libusb_get_config_descriptor(dev, 0, &config);
            if(config != NULL)
            {
                const struct libusb_interface *inter = &config->interface[0];
                const struct libusb_interface_descriptor *interdesc = &inter->altsetting[0];
                for(int j=0; j<(int)interdesc->bNumEndpoints; j++)
                {
                    const struct libusb_endpoint_descriptor *epdesc = &interdesc->endpoint[j];
                    if((epdesc->bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_OUT)
                    {
                        m_nEndpointAddr = epdesc->bEndpointAddress;
                    }
                }
            }
            return true;
        }
    }

    return false;
}
