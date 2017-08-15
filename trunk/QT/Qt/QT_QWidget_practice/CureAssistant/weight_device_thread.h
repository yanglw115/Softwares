/******************************************************************************
 * Copyright(c) 2015 Daboo Technology Co.,Ltd.
 *
 * @FileName: HW_weight_device_thread.h
 * @Author: yangliwei
 * @Version: 1.0
 * @Date: 2015.8.25
 * @Description: 体重测量设备的数据接收与处理
 * @Others:
 * @History:
 *     1、Date: 2015.8.25
 *        Author: yangliwei
 *        Modification: 实现基本功能
*****************************************************************************/

#ifndef _HW_WEIGHT_DEVICE_THREAD_H_
#define _HW_WEIGHT_DEVICE_THREAD_H_

#include <QByteArray>

#include "serial_device_thread.h"

enum enumWeightDataType {
    TYPE_WEIGHT_DEVICE_UNKNOWN = 0,
    TYPE_WEIGHT_DEVICE_OPEN,
    TYPE_WEIGHT_DEVICE_CLOSE,
    TYPE_WEIGHT_WEIGHT_DATA,
    TYPE_WEIGHT_FAT_DATA
};

struct struWeightData {
    float fWeight;
    QByteArray byteArryaFat;
    void clear()
    {
        fWeight = 0;
        byteArryaFat.clear();
    }
};

class CWeightDeviceThread : public CSerialDeviceThread
{
    Q_OBJECT
public:
    explicit CWeightDeviceThread(QObject *parent);
    ~CWeightDeviceThread();

    void sendFatDataGetCmd();

signals:
    /**************************************************************************
     * Function: newWeightData
     * Description: 当接收到下位机上传过来的有效数据时发射给主程序的数据信号
     * Input: 体重数据的类型，体重相关数据
     * Output: 无
     * Return: 空
     *************************************************************************/
    void newWeightData(const enumWeightDataType &type, const struWeightData &data);

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

#endif // _HW_WEIGHT_DEVICE_THREAD_H_
