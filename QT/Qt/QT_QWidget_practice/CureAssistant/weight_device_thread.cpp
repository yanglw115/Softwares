
#include "weight_device_thread.h"
#include "vs_char_set.h"

static const QString g_strWeightDeviceOpen          = "5503025a";
static const QString g_strWeightDeviceClose         = "5503035b";
static const QString g_strWeightDataHead            = "550504";
static const QString g_strFatDataHead               = "550f06";
static const QString g_strWeightDeviceDataHead      = "55";

enum enumWeightIntMaroc {
    LENGTH_OF_WEIGHT_DEVICE_OPEN        = 4,
    LENGTH_OF_WEIGHT_DEVICE_CLOSE       = 4,
    LENGTH_OF_WEIGHT_DEVICE_WEIGHING    = 6,
    LENGTH_OF_WEIGHT_DEVICE_LOCKDATA    = 6,
    LENGTH_OF_WEIGHT_DEVICE_FULL_DATA   = 16,
};

/* 响应体重秤下位机代表“收到的数据有效“ */
const unsigned char g_cmdWeightDataOkAck[]     = {0x55, 0x03, 0x01, 0x59};
/* 响应体重秤下位机代表“收到的数据无效“ */
const unsigned char g_cmdWeightDataErrorAck[]  = {0x55, 0x03, 0x00, 0x58};
//向下位机发送的获取当前体脂数据的指令，其中部分数据需要重新组装
const unsigned char g_cmdGetWeightData[]       = {0x55, 0x0d, 0x10, 0x07, 0xc1, 0x06, 0x06, 0x07, 0xe0, 0x08, 0x0b, 0x01, 0xa4, 0xe5};


CWeightDeviceThread::CWeightDeviceThread(QObject *parent)
    : CSerialDeviceThread(parent)
{
    m_strSerialType = "体重测量仪";
}

CWeightDeviceThread::~CWeightDeviceThread()
{
}

void CWeightDeviceThread::sendFatDataGetCmd()
{
    qDebug() << "准备发送体脂获取指令...";
    writeDataToSerail((const char*)g_cmdGetWeightData, sizeof(g_cmdGetWeightData));
}

void CWeightDeviceThread::handleReceivedSerialData(const QByteArray &byteArrayData)
{
    if (!checkReceivedSerialData(byteArrayData)) {
        return;
    }

    enumWeightDataType type = TYPE_WEIGHT_DEVICE_UNKNOWN;
    struWeightData unionData;
    unionData.clear();

    if (byteArrayData.toHex() == g_strWeightDeviceOpen) {
        qDebug("收到体重秤的开机消息");
        type = TYPE_WEIGHT_DEVICE_OPEN;

    } else if (byteArrayData.toHex() == g_strWeightDeviceClose) {
        qDebug("收到体重秤的关机消息");
        type = TYPE_WEIGHT_DEVICE_CLOSE;

    } else if (byteArrayData.toHex().left(6) == g_strWeightDataHead) {
        unsigned int w = getHighAndLowHexValue(byteArrayData.at(3), byteArrayData.at(4));
        unionData.fWeight = (float)w / 10;
        qDebug("收到体重秤上报的体重数据为: %f", unionData.fWeight);
        type = TYPE_WEIGHT_WEIGHT_DATA;

    } else if (byteArrayData.toHex().left(6) == g_strFatDataHead) {
        qDebug("收到体重秤上报的体脂数据");
        type = TYPE_WEIGHT_FAT_DATA;
        unionData.byteArryaFat = byteArrayData;
    }

    if (TYPE_WEIGHT_DEVICE_UNKNOWN != type) {
        emit newWeightData(type, unionData);
    }
}

void CWeightDeviceThread::handleSerialError(const QString &strErr)
{
    qCritical() << "体重测量仪串口出错: " << strErr;
//    exitThread();
}

void CWeightDeviceThread::handleSerialTimeout(const QString &strTimeout)
{
    QString str;
    str = strTimeout;
}

void CWeightDeviceThread::handleSerailSuccess()
{
    return;
}

void CWeightDeviceThread::handleSerailSendOK(const QByteArray &byteData)
{
    qDebug() << "发送串口数据成功:" << byteData.toHex();
}

bool CWeightDeviceThread::checkReceivedSerialData(const QByteArray &data)
{
    qDebug() << "接收到体重测量仪发给上位机的数据为(Hex):" << data.toHex();
    int dataLength = data.length();
    if (LENGTH_OF_WEIGHT_DEVICE_OPEN == dataLength
            || LENGTH_OF_WEIGHT_DEVICE_CLOSE == dataLength || LENGTH_OF_WEIGHT_DEVICE_WEIGHING == dataLength
            || LENGTH_OF_WEIGHT_DEVICE_LOCKDATA == dataLength ||  LENGTH_OF_WEIGHT_DEVICE_FULL_DATA== dataLength) {

        char checksum = 0;
        for (int i = 0; i < dataLength - 1; ++i)
            checksum += data[i];
        if (checksum == data[dataLength - 1]) {
            qDebug("体重测量仪上报的数据校验成功!");
            writeDataToSerail((const char*)g_cmdWeightDataOkAck, sizeof(g_cmdWeightDataOkAck));
            return true;
        }
        qWarning("体重测量仪上报的数据校验失败!");
    } else {
        qWarning("体重测量仪上报的数据不合法或无法识别!");
    }

    writeDataToSerail((const char*)g_cmdWeightDataErrorAck, sizeof(g_cmdWeightDataErrorAck));
    return false;
}
