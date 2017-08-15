#ifndef CPRINTER_H
#define CPRINTER_H

#include <QObject>

#include "USBControl.h"

#define PRINTER_ASCII_ESC                   0x1b
#define PRINTER_ASCII_FS                    0x1c
#define PRINTER_ASCII_EXCLAMATION           0x21//！号
#define PRINTER_PRINT_NEWLINE               0x0a
#define PRINTER_QUERY                       0x10
#define PRINTER_QUERY_CMD                   0x04
#define PRINTER_QUERY_STATUS                0x02
#define PRINTER_QUERY_ERR                   0x03
#define PRINTER_SET_BITMAP_MODE             0x2a
#define PRINTER_BITMAP_MODE                 0x21
#define PRINTER_SET_PRINT_METHOD            0x33 //程序里用24点双精度模式
#define PRINTER_SET_CHAR_DEFAULT_GAP        0x32
#define PRINTER_INITIALIZE                  0x40
#define PRINTER_PRINT_ADVANCE               0x4a
#define PRINTER_PRINT_ADVANCE_LINE          0x64
#define PRINTER_DOUBLE_CHAR_WIDTH           0x0e
#define PRINTER_NORMAL_CHAR_WIDTH           0x14
#define PRINTER_PRINT_PICTURE               0x2a
#define PRINTER_SET_BARCODE_WIDTH           0x77
#define PRINTER_SET_BARCODE_HEIGHT          0x68
#define PRINTER_QUERY_MANUFACTURE_INFO      0x49
#define PRINTER_CUT_PAPER                   0x56
#define PRINTER_GS                          0x1d
#define PRINTER_BARCODE_MODE                0x49
#define PRINTER_PRINT_BARCODE               0x6b
#define PRINTER_US                          0x1f
#define PRINTER_PRINT_QR                    0x1c

#define PRINTER_FS_DOUBLE_WIDTH             0x04
#define PRINTER_FS_DOUBLE_HEIGHT            0x08

#define PRINTER_SENDBUF_LEN                 1024
#define PRINTER_MAX_LATIN_CHARS_PER_LINE    32
#define PRINTER_MAX_CN_CHARS_PER_LINE       16

#define PRINTER_VENDOR_ID                   0x0493
#define PRINTER_PRODUCT_ID                  0x8760

class CPrinter : public QObject
{
    Q_OBJECT
public:
    explicit CPrinter(QObject *parent = 0);
    ~CPrinter();

    //初始化打印机
    void init();
    //清除打印缓冲区中的数据，复位打印机打印参数到当打印机缺省参数
    void clear();
    //打印并进纸n个垂直点距，0 ≤ n ≤ 255，一个垂直点距为0.33mm
    void printAdvance(int n);
    //打印并进纸n行
    void printAdvanceLine(int n);
    //打印换行
    void printEnter();
    //打印一行内容
    void printLineMsg(const QString &strMsg);
    //打印二维码
    void printQrCode(const uchar* pCode, int nLen);
    //打印条形码
    void printBarCode(const uchar* pCode, int nLen);
    //打印位图
    void printBitmap(const uchar* pData, int nWidth, int nHeight);
    //切纸
    void cutPaper();
signals:

public slots:

private:
    CUSBControl *m_pUsbControl;
};

#endif // CPRINTER_H
