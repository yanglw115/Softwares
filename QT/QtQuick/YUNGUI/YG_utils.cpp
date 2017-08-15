
#include <QApplication>
#include <QCryptographicHash>
#include <QFile>
#include <QProcess>
#include <QString>
#include <QtDebug>
#include <QTimer>

#include "YG_des_source.h"
#include "YG_utils.h"

CUtils::CUtils()
{

}

CUtils::~CUtils()
{

}

QString CUtils::floatToString(float data, int precision, char format)
{
    QString strValue;
    strValue = strValue.setNum(data, format, precision);
    return strValue;
}

void CUtils::changeADFile(const QString &strOldFileName, const QString &strNewFileName)
{
    QString strOldFilePath = getFileFullPath(strOldFileName);
    QString strNewFilePath = getFileFullPath(strNewFileName);
    if (QFile::exists(strOldFilePath)) {
        if (QFile::remove(strOldFilePath)) {
            qDebug() << "删除旧的广告文件成功: " << strOldFilePath;
        } else {
            qCritical() << "删除旧的广告文件失败: " << strOldFilePath;
        }
    }
    if (QFile::exists(strNewFilePath)) {
        /* 这里是把新文件改名为老文件名 */
        if (QFile::rename(strNewFilePath, strOldFilePath)) {
            qDebug() << "替换广告文件成功，新的文件为: " << strOldFilePath;
        } else {
            qCritical() << "替换广告文件失败!";
        }
    } else {
        qWarning() << "替换广告文件失败，因为文件不存在: " << strNewFilePath;
    }
}

void CUtils::insertData(float array[], int arrayLength, float newData)
{
    qDebug() << "插入前的数组数据为：";
    for (int i = 0; i < arrayLength; ++i)
        qDebug() << array[i];

    for (int i = 0; i < (arrayLength - 1); ++i) {
        array[i] = array[i + 1];
    }
    array[arrayLength - 1] = newData;

    qDebug() << "插入后的数组数据为：";
    for (int i = 0; i < arrayLength; ++i)
        qDebug() << array[i];
}

bool CUtils::testDataStable(float array[], int arrayLength, float deviation)
{
    for (int i = 0; i <arrayLength - 1; ++i) {
        if (qAbs((array[i] - array[i + 1])) > deviation)
            return false;
    }
    return true;
}

bool CUtils::execApp(const QString &appPath)
{
    if (appPath.isEmpty() || appPath.isNull()) {
        qCritical() << "execApp执行失败,要执行的程序路径为空!";
        return false;
    }

    if(!QFile::exists(appPath)) {
        qCritical() << "execApp执行失败,文件不存在,要执行的程序路径:" << appPath;
        return false;
    }

    /* 这里不能使用QProcess processApp; 如果使用QProcess processApp;
     * 这个接口执行完成的时候 拉起的程序直接退出了 只能用指针 */
    QProcess *processApp = new QProcess;
    processApp->start(appPath);

    while (false == processApp->waitForStarted(5000)) {
        qCritical( )<< "execApp执行失败 在5秒内等待其start未成功.";
        processApp->close();
        return false;
    }

    //processApp->close();
    qDebug() << "execApp执行成功";
    return true;
}

QString CUtils::getFileFullPath(const QString &strFilename)
{
    QString strFileFullPath = QApplication::applicationDirPath() + "/" + strFilename;
    return strFileFullPath;
}

QString CUtils::calculateMD5ForString(const QString &strData)
{
    QString strReturn = "";
    if (!strData.isEmpty()) {
        strReturn = QString(QCryptographicHash::hash(strData.toLocal8Bit(),
                                                     QCryptographicHash::Md5).toHex());
    }
    return strReturn;
}

QString CUtils::calculateMD5ForFile(const QString &filePath)
{
    QString strReturn = "";
    if (!filePath.isEmpty() && QFile::exists(filePath)) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QCryptographicHash hash(QCryptographicHash::Md5);
            /* 此方法需要验证是否支持大文件 */
            hash.addData(&file);
            strReturn = QString(hash.result().toHex());
            file.close();
        } else {
            qWarning() << "文件打开失败，无法进行MD5取值！";
        }
    }
    return strReturn;
}

void CUtils::disconnectTimerTimeout(QTimer *timer)
{
    if (timer->isActive())
        timer->stop();
    timer->disconnect(SIGNAL(timeout()));
}

void CUtils::stopSpecificTimer(QTimer *timer)
{
    if (timer->isActive())
        timer->stop();
}

bool CUtils::encryptDataByDES(const QByteArray &inData, const QString &inStrKey, QByteArray &outData)
{
   if (inData.isEmpty() || inStrKey.isEmpty()) {
       qWarning() << "DES加密的原数据或密钥为空,无法进行加密!";
       return false;
   }

   if (8 != inStrKey.length()) {
       qWarning() << "密钥长度不够8个字符,无法进行正常加密!";
       return false;
   }

   if (0 != (inData.length() % 8)) {
       /* 加密的数据长度必须是8字节的整数倍,这里不做容错处理 */
       qWarning() << "加密的数据不合法,长度不等于8N,加密失败!";
       return false;
   }

   outData.resize(inData.length());
   outData.fill(0);
   /* 加密过程一次只能对8个字节进行 */
   for (int i = 0; i < inData.length() / 8; ++i) {
       encryptDES((unsigned char*)(inData.data() + i * 8),
                  (unsigned char*)(inStrKey.toLocal8Bit().data()),
                  (unsigned char*)outData.data() + i * 8);
   }
   return true;
}

bool CUtils::decryptDataByDES(const QByteArray &inData, const QString &inStrKey, QByteArray &outData)
{
    if (inData.isEmpty() || inStrKey.isEmpty()) {
        qWarning() << "DES解密的原数据或密钥为空,无法进行解密!";
        return false;
    }

    if (8 != inStrKey.length()) {
        qWarning() << "密钥长度不够8个字符,无法进行正常解密!";
        return false;
    }

    if (0 != (inData.length() % 8)) {
        /* 解密的数据长度必须是8字节的整数倍,这里不做容错处理 */
        qWarning() << "解密的数据不合法,长度不等于8N,解密失败!";
        return false;
    }

    outData.resize(inData.length());
    outData.fill(0);
    /* 解密过程一次只能对8个字节进行 */
    for (int i = 0; i < inData.length() / 8; ++i) {
        decryptDES((unsigned char*)(inData.data() + i * 8),
                   (unsigned char*)(inStrKey.toLocal8Bit().data()),
                   (unsigned char*)outData.data() + i * 8);
    }
    return true;
}

