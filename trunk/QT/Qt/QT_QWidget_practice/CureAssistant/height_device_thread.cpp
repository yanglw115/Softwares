
#include "height_device_thread.h"
#include "vs_char_set.h"

static const QString g_strHeightDataHead            = "fec2";
static bool g_bGetStandardData                      = false;

/* 主动获取标高时，返回的标高以fe c6开头，后面两个固定是00 00，再后面两个表示十进制2107即2107mm */
static const QString g_strStandardDataHead          = "fec6"; //FE C6 00 00 21 07 EF DB
/* 主动设置标高时的返回 */
static const QString g_strStandardACKHead           = "fec4"; //fe c4 07 d0 20 00 ef b2
static const QString g_strHeightDeviceOpenOK        = "fea200000000ef";

//向下位机发送打开身高测量设备的指令
const unsigned char g_cmdOpenHeightDevice[]    = {0xFE, 0xA1, 0x00, 0x00, 0x00, 0x00, 0xEF, 0x8E};
//向下位机发送的获取当前身高数据的指令
const unsigned char g_cmdGetHeightData[]       = {0xFE, 0xC1, 0x00, 0x00, 0x00, 0x00, 0xEF, 0xAE};
//向下位机发送的获取当前体脂数据的指令，其中部分数据需要重新组装
const unsigned char g_cmdGetWeightData[]       = {0x55, 0x0d, 0x10, 0x07, 0xc1, 0x06, 0x06, 0x07, 0xdf, 0x06, 0x0b, 0x01, 0xaa, 0x00};
//获取当前标高
const unsigned char g_cmdGetStandardHeight[]   = {0xfe, 0xc5, 0x00, 0x00, 0x00, 0x00, 0xef, 0xb2};
//设置标高
const unsigned char g_cmdSetStandardHeight[]   = {0xfe, 0xc3, 0x00, 0x00, 0x20, 0x00, 0xef, 0xb2};
//如果设置成功，则返回fe c4 07 d0 20 00 ef b2,其中07 d0是20000的十六进制

enum enumHeightIntMacro {
    LENGTH_OF_HEIGHT_DEVICE_DATA        = 8,
};

CHeightDeviceThread::CHeightDeviceThread(QObject *parent)
    : CSerialDeviceThread(parent)
{
    m_strSerialType = "身高测量仪";
}

CHeightDeviceThread::~CHeightDeviceThread()
{

}

void CHeightDeviceThread::sendMeasureData()
{
    qDebug("发送身高测量指令...");
    writeDataToSerail((const char*)g_cmdGetHeightData, sizeof(g_cmdGetHeightData));
}

void CHeightDeviceThread::getStandardData()
{
    qDebug("发送获取标高指令...");
    g_bGetStandardData = true;
    writeDataToSerail((const char*)g_cmdGetStandardHeight, sizeof(g_cmdGetStandardHeight));
}

void CHeightDeviceThread::setStandardData(const int &height)
{
    QByteArray byteData = QByteArray::fromHex(QString("").setNum(height).toLocal8Bit());

    char dataAraay[sizeof(g_cmdSetStandardHeight)] = {0};
    memcpy(dataAraay, g_cmdSetStandardHeight, sizeof(g_cmdSetStandardHeight));
    dataAraay[4] = byteData.at(0);
    dataAraay[5] = byteData.at(1);

    qDebug("发送设置标高指令...");
    writeDataToSerail((const char*)dataAraay, sizeof(dataAraay));

}

void CHeightDeviceThread::handleReceivedSerialData(const QByteArray &byteArrayData)
{
//    if (!checkReceivedSerialData(byteArrayData)) {
//        return;
//    }
    qDebug() << "接收到身高测量仪发给上位机的数据为(Hex):" << byteArrayData.toHex();
    if (byteArrayData.toHex() == g_strHeightDeviceOpenOK) {
        qDebug("身高测量仪打开成功");
        ;
    } else if (byteArrayData.toHex().left(4) == g_strHeightDataHead) {
        /* 用两个字节表示身高的值，一个高字节，一个低字节，需要计算得出其值 */
        unsigned int h = getHighAndLowHexValue(byteArrayData.at(4), byteArrayData.at(5));
        /* 计算得出来的值单位是毫米，这里转换为后续用的厘米 */
        float height = (float)h / 10;
        qDebug("身高测量仪上报的身高数据为: %f", height);
        emit sigNewHeightData(height);
    } else if ((byteArrayData.toHex().left(4) == g_strStandardDataHead) || g_bGetStandardData) {
        QString strData(byteArrayData.toHex());
        int h = strData.mid(strData.length() - 8, 4).toInt();
        qDebug() << "获取到的标高为:" << h;
        g_bGetStandardData = false;
        emit sigGetStandardHeight(h);

    } else if (byteArrayData.toHex().left(4) == g_strStandardACKHead) {
        qDebug() << "设置标高成功!";
        emit sigSetStandardSuccess();
    }
}

void CHeightDeviceThread::handleSerialError(const QString &strErr)
{
    qCritical() << "身高设备串口出错: " << strErr;
//    exitThread();
}

void CHeightDeviceThread::handleSerialTimeout(const QString &strTimeout)
{
    QString str;
    str = strTimeout;
}

void CHeightDeviceThread::handleSerailSuccess()
{
    qDebug() << "身高设备串口打开成功!";
    return;
}

void CHeightDeviceThread::handleSerailSendOK(const QByteArray &byteData)
{
    qDebug() << "发送串口数据成功:" << byteData.toHex();
}

bool CHeightDeviceThread::checkReceivedSerialData(const QByteArray &data)
{
    qDebug() << "接收到身高测量仪发给上位机的数据为(Hex):" << data.toHex();
    int dataLength = data.length();
    if (LENGTH_OF_HEIGHT_DEVICE_DATA == dataLength) {
            char checksum = 0;
            for (int i = 0; i < dataLength - 1; ++i)
                checksum += data[i];
            if (checksum == data[dataLength - 1]) {
                qDebug("身高数据校验成功!");
                return true;
            }
        qWarning("身高数据校验失败!");
    } else {
        qWarning("身高数据不合法或无法识别!");
    }
    return false;
}

