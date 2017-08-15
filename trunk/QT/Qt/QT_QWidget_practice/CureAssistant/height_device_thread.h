/******************************************************************************
 * Copyright(c) 2015 Daboo Technology Co.,Ltd.
 *
 * @FileName: HW_height_device_thread.h
 * @Author: yangliwei
 * @Version: 1.0
 * @Date: 2015.8.25
 * @Description: 身高测量设备的数据接收与处理
 * @Others:
 * @History:
 *     1、Date: 2015.8.25
 *        Author: yangliwei
 *        Modification: 实现基本功能
*****************************************************************************/

#ifndef _HW_HEIGHT_DEVICE_THREAD_H_
#define _HW_HEIGHT_DEVICE_THREAD_H_

#include "serial_device_thread.h"

enum enumHeightDataType {
    TYPE_HEIGHT_DEVICE_DATA = 0,
    TYPE_HEIGHT_DEVICE_OPEN
};

class CHeightDeviceThread : public CSerialDeviceThread
{
    Q_OBJECT
public:
    explicit CHeightDeviceThread(QObject *parent);
    ~CHeightDeviceThread();
    void sendMeasureData();
    void getStandardData();
    void setStandardData(const int &);

signals:
    /**************************************************************************
     * Function: newHeightData
     * Description: 当接收到下位机上传过来的有效数据时发射给主程序的数据信号
     * Input: 身高数据的类型，身高相关数据
     * Output: 无
     * Return: 空
     *************************************************************************/
    void sigNewHeightData(const float &fData);

    void sigGetStandardHeight(const int &nData);
    void sigSetStandardSuccess();

protected slots:
    /**************************************************************************
     * Function: handleReceivedSerialData
     * Description: 处理下位机上传的数据。对数据进行有效性检查和上报给主进程等。
     * Input: 下位机上传的数据。
     * Output: 无
     * Return: 空
     *************************************************************************/
    virtual void handleReceivedSerialData(const QByteArray &);

    /**************************************************************************
     * Function: handleSerialError
     * Description: 串口打开或读写出错槽函数
     * Input: 错误信息
     * Output: 无
     * Return: 空
     *************************************************************************/
    virtual void handleSerialError(const QString &);

    /**************************************************************************
     * Function: handleSerialTimeout
     * Description: 串口读写出超时处理槽函数
     * Input: 错误信息
     * Output: 无
     * Return: 空
     *************************************************************************/
    virtual void handleSerialTimeout(const QString &);

    virtual void handleSerailSuccess();

    virtual void handleSerailSendOK(const QByteArray &byteData);

protected:

    /**************************************************************************
     * Function: checkReceivedSerialData
     * Description: 对下位机上传的数据进行效验
     * Input: 下位机上传的数据
     * Output: 无
     * Return: 有效数据返回true,无效数据返回false
     *************************************************************************/
    virtual bool checkReceivedSerialData(const QByteArray &data);

private:

};

#endif // _HW_HEIGHT_DEVICE_THREAD_H_
