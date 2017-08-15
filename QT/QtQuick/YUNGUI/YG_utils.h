#ifndef _YG_UTILS_H_
#define _YG_UTILS_H_

class QByteArray;
class QString;
class QTimer;

class CUtils
{
public:
    CUtils();
    ~CUtils();

    /**************************************************************************
     * Function: floatToString
     * Description: 将浮点型数据转换为字符型
     * Input: 浮点数据、精确度和浮点数据显示格式
     * Output: 无
     * Return: 转换后的字符串
     *************************************************************************/
    static QString floatToString(float data, int precision = 1, char format = 'f');

    /**************************************************************************
     * Function: disconnectTimerTimeout
     * Description: 对超时定时器进行disconnect操作，因为这里是一个定时器可能同时服务多个
     * 对象。
     * Input: 定时器指针
     * Output: 无
     * Return: 空
     *************************************************************************/
    static void disconnectTimerTimeout(QTimer *timer);

    /**************************************************************************
     * Function: stopSpecificTimer
     * Description: 使特定的定时器停止工作
     * Input: 定时器指针
     * Output: 无
     * Return: 空
     *************************************************************************/
    static void stopSpecificTimer(QTimer *timer);

    /**************************************************************************
     * Function: changeADFile
     * Description: 改变广告文件，主要用于升级广告内容。
     * Input: 旧文件名称和新文件名称
     * Output: 无
     * Return: 空
     *************************************************************************/
    static void changeADFile(const QString &strOldFileName, const QString &strNewFileName);

    /**************************************************************************
     * Function: getHighAndLowHexValue
     * Description: 内联函数。用于计算一个高位与一个低位的十六进制的值
     * Input: 高位对应的值与低位对应的值
     * Output: 无
     * Return: 计算之后的结果
     *************************************************************************/
    static inline int getHighAndLowHexValue(char high, char low)
    {
        return (((unsigned char)high << 8) + (unsigned char)low);
    }

    static inline float calculateBmiAndFat(float fatRate, float frBase, float frFactor, float bfBase, float bfFactor)
    {
        return ((((fatRate - frBase) * bfFactor) / frFactor) + bfBase);
    }

    /**************************************************************************
     * Function: insertData
     * Description: 将数组中的数据前移，数组最后插入新数据
     * Input: 数组、数组长度及新数据
     * Output: 新的数组
     * Return: 无
     *************************************************************************/
    static void insertData(float array[], int arrayLength, float newData);

    /**************************************************************************
     * Function: testDataStable
     * Description: 判断数组中的数据是否稳定，即相邻数据的误差是否都小于默认误差值
     * Input: 数组、数组长度及默认误差值
     * Output: 无
     * Return: 数组数据是否稳定
     *************************************************************************/
    static bool testDataStable(float array[], int arrayLength, float deviation);

    /**************************************************************************
     * Function: execApp
     * Description: 启动其他进程
     * Input: 进程所在路径
     * Output: 无
     * Return: 启动成功返回真,失败返回假
     *************************************************************************/
    static bool execApp(const QString &appPath);

    /**************************************************************************
     * Function: getFileFullPath
     * Description: 获取文件的绝对路径
     * Input: 文件名称
     * Output: 无
     * Return: 文件的绝对路径
     *************************************************************************/
    static QString getFileFullPath(const QString &);

    /**************************************************************************
     * Function: calculateMD5ForString
     * Description: 计算字符串的MD5值
     * Input: 字符串
     * Output: 无
     * Return: MD5值
     *************************************************************************/
    static QString calculateMD5ForString(const QString &);

    /**************************************************************************
     * Function: calculateMD5ForFile
     * Description: 计算文件的MD5值
     * Input: 文件所在路径
     * Output: 无
     * Return: MD5值
     *************************************************************************/
    static QString calculateMD5ForFile(const QString &filePath);

    /**************************************************************************
     * Function: encryptDataByDES
     * Description: DES加密
     * Input: inData:明文
     *        inStrKey:密钥
     *        outData:密文保存所在数据
     * 补充:(解密也是一样)
     *     1.项目中如果是对MD5字符串进行加密,则这样使用:
     *      QString strKey = "12345678";
     *      QString strMD5 = getMD5();
     *      QByteArray byteArrayText = QByteArray::fromHex(strMD5.toLocal8Bit());
     *      QByteArray byteArrayCipher = "";
     *      encryptDataByDES(byteArrayText, strKey, byteArrayCipher);
     *     2.密钥在定义的时候字符长度只能是8. 中间不能夹杂"\0",这个代表字符串结束,例如:
     *      QString strKey = "*(DY$\0U"
     *      就是错误的,因为中间有\0,这样字符串实际长度只有5个字符;
     *     3.最终的数据需要这样使用:QString(byteArrayCipher.toHex())
     * Output: 密文
     * Return: 加密成功返回真,否则返回假
     *************************************************************************/
    static bool encryptDataByDES(const QByteArray &inData, const QString &inStrKey, QByteArray &outData);

    /**************************************************************************
     * Function: decryptDataByDES
     * Description: DES解密
     * Input: inData:密文
     *        inStrKey:密钥
     *        outStrData:明文保存所在数据
     * Output: 明文
     * Return: 解密成功返回真,否则返回假
     *************************************************************************/
    static bool decryptDataByDES(const QByteArray &inData, const QString &inStrKey, QByteArray &outStrData);
};

#endif // _YG_UTILS_H_

