#ifndef _DES_SOURCE_H_
#define _DES_SOURCE_H_

#include <QObject>
#include <QString>

class CMyDES: public QObject
{
    Q_OBJECT
public:
    explicit CMyDES(QObject *parent = 0);
    ~CMyDES();

    Q_INVOKABLE static QString md5ForFile(const QString strPath);
    Q_INVOKABLE static QString encryptDataByDES(const QString inData, const QString inStrKey);
    Q_INVOKABLE static QString decryptDataByDES(const QString inData, const QString inStrKey);
};

#endif // _DES_SOURCE_H_
