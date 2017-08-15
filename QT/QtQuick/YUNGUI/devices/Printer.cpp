#include "Printer.h"

#include <QTextCodec>

#include <stdlib.h>

CPrinter::CPrinter(QObject *parent) : QObject(parent)
{
    m_pUsbControl = new CUSBControl();
}

CPrinter::~CPrinter()
{
    m_pUsbControl->closeDevice();

    if(m_pUsbControl != NULL)
    {
        delete m_pUsbControl;
        m_pUsbControl = NULL;
    }
}

//初始化打印机
void CPrinter::init()
{
    if(m_pUsbControl->openDevice(PRINTER_VENDOR_ID, PRINTER_PRODUCT_ID) != 0)
    {
        qDebug("Open printer faild!");
        return;
    }

    clear();
}

//清除打印缓冲区中的数据，复位打印机打印参数到当打印机缺省参数
void CPrinter::clear()
{
    uchar data[2] = {PRINTER_ASCII_ESC, PRINTER_INITIALIZE};
    if(m_pUsbControl->sendData(data, 2) < 0)
    {
        qDebug("Send command error");
    }
}

//打印并进纸n个垂直点距，0 ≤ n ≤ 255，一个垂直点距为0.33mm
void CPrinter::printAdvance(int n)
{
    uchar data[3] = {PRINTER_ASCII_ESC, PRINTER_PRINT_ADVANCE, n};
    if(m_pUsbControl->sendData(data, 3) < 0)
    {
        qDebug("Send command error");
    }
}

//打印并进纸n行
void CPrinter::printAdvanceLine(int n)
{
    uchar data[3] = {PRINTER_ASCII_ESC, PRINTER_PRINT_ADVANCE_LINE, n};
    if(m_pUsbControl->sendData(data, 3) < 0)
    {
        qDebug("Send command error");
    }
}

//打印换行
void CPrinter::printEnter()
{
    uchar data = PRINTER_PRINT_NEWLINE;
    if(m_pUsbControl->sendData(&data, 1) < 0)
    {
        qDebug("Send command error");
    }
}

//打印一行内容
void CPrinter::printLineMsg(const QString &strMsg)
{
    QTextCodec *codec = QTextCodec::codecForName("GB18030");
    QByteArray bytes = codec->fromUnicode(strMsg);
    if(m_pUsbControl->sendData((uchar*)bytes.data(), bytes.size()) < 0)
    {
        qDebug("Send command error");
    }
}

//打印二维码
void CPrinter::printQrCode(const uchar* pCode, int nLen)
{

}

//打印条形码
void CPrinter::printBarCode(const uchar* pCode, int nLen)
{

}

//打印位图
void CPrinter::printBitmap(const uchar* pData, int nWidth, int nHeight)
{
        const uchar *ptr = pData;
        uchar data[3] = {0x00};
        uchar escBmp[5] = {PRINTER_ASCII_ESC,PRINTER_SET_BITMAP_MODE};
        //M
        escBmp[2] = 0x21;
        //nL, nH
        escBmp[3] = (nWidth % 256) & 0xFF;
        escBmp[4] = (nWidth / 256) & 0xFF;

        // 每行进行打印
        for (int i = 0; i < nHeight  / 24 + 1; i++)
        {
            m_pUsbControl->sendData(escBmp, 5);

            for (int j = 0; j < nWidth; j++)
            {
                for (int k = 0; k < 24; k++)
                {
                    if (((i * 24) + k) < nHeight)
                    {
                        int pixelColor = * (ptr+nWidth*((i*24)+k) + j);//bmp.getPixel(j, (i * 24) + k);
                        if (pixelColor != -1)
                        {
                            data[k / 8] += (128 >> (k % 8)) & 0xFF;
                        }
                    }
                }

                m_pUsbControl->sendData(data, 3);
                // 重置参数
                data[0] = 0x00;
                data[1] = 0x00;
                data[2] = 0x00;
            }
            //换行
            /*
            byte[] byte_send1 = new byte[2];
            byte_send1[0] = 0x0d;
            byte_send1[1] = 0x0a;
            socketOut.write(byte_send1);
            */
            printEnter();
        }
}

//切纸
void CPrinter::cutPaper()
{
    uchar data[3] = {PRINTER_GS, PRINTER_CUT_PAPER, 0};
    if(m_pUsbControl->sendData(data, 3) < 0)
    {
        qDebug("Send command error");
    }
}
