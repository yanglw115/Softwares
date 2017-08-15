
#include "serial_device_thread.h"
#include "vs_char_set.h"

#define SERIAL_WAIT_TIMEOUT     200 //200ms

const QString g_strArraySerialError[] = {"NoError",
                                       "DeviceNotFoundError",
                                       "PermissionError",
                                       "OpenError",
                                       "NotOpenError",
                                       "ParityError",
                                       "FramingError",
                                       "BreakConditionError",
                                       "WriteError",
                                       "ReadError",
                                       "ResourceError",
                                       "UnsupportedOperationError",
                                       "TimeoutError",
                                       "UnknownError"
                                      };

CSerialDeviceThread::CSerialDeviceThread(QObject *parent)
    : QThread(parent)
    , m_strPortName("")
    , m_uBaudrate(9600)
    , m_iWaitTimeout(SERIAL_WAIT_TIMEOUT)
    , m_bQuit(true)
    , m_bOpenSerailOK(false)
    , m_strSerialType("")
{
}

CSerialDeviceThread::~CSerialDeviceThread()
{
    m_mutex.lock();
    m_bQuit = true;
    m_cond.wakeOne();
    m_mutex.unlock();
    wait();
}

void CSerialDeviceThread::init()
{
    connect(this, SIGNAL(signalResponse(const QByteArray&)), this, SLOT(handleReceivedSerialData(const QByteArray&)));
    connect(this, SIGNAL(signalError(const QString&)),       this, SLOT(handleSerialError(const QString&)));
    connect(this, SIGNAL(signalTimeout(const QString&)),     this, SLOT(handleSerialTimeout(const QString&)));
    connect(this, SIGNAL(signalSuccess()),                   this, SLOT(handleSerailSuccess()));
    connect(this, SIGNAL(signalSendOK(const QByteArray&)),   this, SLOT(handleSerailSendOK(const QByteArray&)));
}

bool CSerialDeviceThread::openDevice()
{
    if (m_bQuit) {
        qDebug() << "正在启动串口线程:" << m_strSerialType;
        start();
    } else {
        qWarning() << "串口设备正在运行，无需打开:" << m_strSerialType;
    }
    return true;
}

bool CSerialDeviceThread::closeDevice()
{
    if (!m_bQuit) {
        m_mutex.lock();
        m_bQuit = true;
        m_mutex.unlock();
    }
    return true;
}

void CSerialDeviceThread::setSerialPortName(const QString &strName)
{
    m_strPortName = strName;
}

void CSerialDeviceThread::writeDataToSerail(const char data[], int length)
{
    m_mutex.lock();
    if (!m_byteArrayRequest.isEmpty()) {
        m_byteArrayRequest.clear();
    }
    QByteArray byteArray(data, length);
    m_byteArrayRequest = byteArray;
    m_mutex.unlock();
}

void CSerialDeviceThread::run()
{
    while (1) {
        QSerialPort serial;
        if (m_bQuit) {
            serial.close();
            serial.setPortName(m_strPortName);
            serial.setBaudRate(m_uBaudrate);

            if (!serial.open(QIODevice::ReadWrite)) {
                /** 出错处理函数里面需要处理,或者使退出线程,或者改变串口的配置,如果要改变串口的配置,即改变
                     * m_strPortName的值,则需要在handleSerialError槽函数中处理 */
                emit signalError(QString("无法打开%1: %2").arg(m_strPortName).arg(g_strArraySerialError[serial.error()]));
                m_bQuit = true;
                return;
            } else {
                /** 设置m_bOpenSerailOK成员变量的目的是考虑有些串口配置即m_strPortName会动态改变,改变时需要
                     * 置m_bOpenSerailOK = false,以重新打开新的串口 */
                m_bOpenSerailOK = true;
                m_bQuit = false;
                emit signalSuccess();
            }
        } else {
            return;
        }

        // write request & read
        /* 这里可以不对m_bOpenSerailOK加锁,因为while里面有超时机制,很快又会重新开始判断 */
        while (m_bOpenSerailOK) {
            QByteArray requestData;
            requestData.clear();

            m_mutex.lock();
            if (m_bQuit) {
                serial.close(); // close serial port
                m_mutex.unlock();
                return;
            }
            if (!m_byteArrayRequest.isEmpty())
                requestData = m_byteArrayRequest;
            m_byteArrayRequest.clear();
            m_mutex.unlock();

            if (!requestData.isEmpty()) {
                serial.write(requestData);
                if (serial.waitForBytesWritten(m_iWaitTimeout)) {
                    emit signalSendOK(requestData);
                } else {
                    emit signalTimeout(QString("写串口数据超时 %1").arg(QTime::currentTime().toString()));
                }
            } else {
                //qWarning() << "将要写入串口的数据为空!";
            }

            if (serial.waitForReadyRead(m_iWaitTimeout)) {
                QByteArray responseData = serial.readAll();
                while (serial.waitForReadyRead(m_iWaitTimeout))
                    responseData += serial.readAll();
                //qDebug() << m_strSerialType <<":接收到测量仪上报的原始数据为:"<<responseData.toHex();
                emit this->signalResponse(responseData);

            } else {
                emit signalTimeout(QString("等待读取串口数据超时 %1").arg(QTime::currentTime().toString()));
            }
        }
    }
}

void CSerialDeviceThread::exitThread()
{
    qDebug() << m_strSerialType << ":准备退出线程！";
    m_mutex.lock();
    m_bQuit = true;
    m_mutex.unlock();
    exit();
}

