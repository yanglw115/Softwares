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

    Q_INVOKABLE QString md5ForFile(const QString strPath);
    Q_INVOKABLE QString encryptDataByDES(const QString inData, const QString inStrKey);
    Q_INVOKABLE QString decryptDataByDES(const QString inData, const QString inStrKey);
};

#ifdef __cplusplus
extern "C" {

/*function prototypes*/
void encryptDES(unsigned char m_bit[8], unsigned char k_bit[8], unsigned char e_bit[8]);
void decryptDES(unsigned char m_bit[8] , unsigned char k_bit[8] , unsigned char e_bit[8]);

//void byteToBit(unsigned char byte[64], unsigned char bit[8]);
//void bitToByte(unsigned char bit[8], unsigned char byte[64]);
//void generateRoundKey(unsigned char oldkey[8], unsigned char newkey[16][8]);
static void boxSReplace(unsigned char s_bit[8]);
/*function prototypes*/

}
#endif // __cplusplus

#endif // _DES_SOURCE_H_
