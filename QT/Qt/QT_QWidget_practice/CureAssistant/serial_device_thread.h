/******************************************************************************
 * Copyright(c) 2015 Daboo Technology Co.,Ltd.
 *
 * @FileName: HW_serial_device_thread.h
 * @Author: yangliwei
 * @Version: 1.0
 * @Date: 2015.7.12
 * @Description: Linux健康秤串口设备线程类，每个实例代表一个串口设备，在主循环中不断监控将
 *     要写入串口的数据及从串口读取的数据。通过主程序中的信号与槽方法处理即将写入的数据与读取
 *     到的数据。
 * @Others:
 * @History:
 *     1、Date: 2015.7.12
 *        Author: yangliwei
 *        Modification: 实现基本功能
*****************************************************************************/

#ifndef _HW_SERIAL_DEVICE_THREAD_H_
#define _HW_SERIAL_DEVICE_THREAD_H_

#include <QMutex>
#include <QtDebug>
#include <QThread>
#include <QTime>
#include <QtSerialPort/QSerialPort>
#include <QWaitCondition>

#ifdef Q_OS_UNIX
const QString g_strSerialPortNamePrefix        = "ttyS";
#else
const QString g_strSerialPortNamePrefix		   = "COM";
#endif

/******************************************************************************
 * Class name: SerialDeviceThread
 * Description: 串口设备所用线程类。主要用于监听串口设备上报的数据及将要写入串口的数据。
 * Others:
 *****************************************************************************/
class CSerialDeviceThread : public QThread
{
    Q_OBJECT
public:
    /**************************************************************************
     * Function: SerialDeviceThread
     * Description: 构造函数
     * Input: parent为父控件
     *************************************************************************/
    explicit CSerialDeviceThread(QObject *parent = 0);

    /**************************************************************************
     * Function: ~SerialDeviceThread
     * Description: 析构函数
     *************************************************************************/
    ~CSerialDeviceThread();

    /**************************************************************************
     * Function: init
     * Description: 初始化线程，主要是初始化串口的端口及波特率
     * Input: 串口信息所在配置文件名和对应配置文件中的组名
     * Output: 无
     * Return: 空
     *************************************************************************/
    void init();

    void setSerialPortName(const QString &);

    /**************************************************************************
     * Function: writeDataToSerail
     * Description: 将要写入串口的数据准备好，串口循环会周期性地查看如果请求数据不为空，则
     * 将请求数据发送出去
     * Input: 将要入写入串口的数据及长度
     * Output: 无
     *************************************************************************/
    void writeDataToSerail(const char data[], int length);

    inline int getHighAndLowHexValue(char high, char low)
        {
            return (((unsigned char)high << 8) + (unsigned char)low);
        }

signals:
    /**************************************************************************
     * Function: response
     * Description: 信号函数。如果接收到下位机发送的串口数据，则发射此信号
     * Input: 接收到的下位机发送过来的数据
     * Output: 无
     *************************************************************************/
    void signalResponse(const QByteArray &s);

    /**************************************************************************
     * Function: error
     * Description: 信号函数。如果串口相关操作出错，则发射此信号
     * Input: 串口操作出错的相关信息
     * Output: 无
     *************************************************************************/
    void signalError(const QString &s);

    void signalSuccess();

    void signalSendOK(const QByteArray &);

    /**************************************************************************
     * Function: timeout
     * Description: 信号函数。串口读写超时时发射此信号
     * Input: 串口读写超时相关信息
     * Output: 无
     *************************************************************************/
    void signalTimeout(const QString &s);

public slots:
    bool openDevice();
    bool closeDevice();
    virtual void handleReceivedSerialData(const QByteArray &) = 0;
    virtual void handleSerialError(const QString &) = 0;
    virtual void handleSerialTimeout(const QString &) = 0;
    virtual void handleSerailSuccess() = 0;
    virtual void handleSerailSendOK(const QByteArray &) = 0;
    virtual void exitThread();

protected:
    virtual bool checkReceivedSerialData(const QByteArray &data) = 0;
    void run();

public:
    QString m_strPortName;
    unsigned int m_uBaudrate;
    QByteArray m_byteArrayRequest;      //上位机向下位要发送的数据
    int m_iWaitTimeout;
    QMutex m_mutex;
    QWaitCondition m_cond;
    bool m_bQuit;
    bool m_bOpenSerailOK;               // 串口打开成功与否
    QString m_strSerialType;            // 串口类型,主要用于辅助打印
};

#endif // _HW_SERIAL_DEVICE_THREAD_H_
