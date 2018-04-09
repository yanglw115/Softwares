#include "mainDialog.h"
#include "ui_mainDialog.h"

#include "jw_des.h"

#include <QtDebug>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>


#include <winsock2.h>
#include <iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#define UDP_PORT 9559
#define TCP_PORT 9559

unsigned char *DES_KEY = (unsigned char *)"~^3*H(@K";

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    m_pTcpSocket = new QTcpSocket(this);
    m_pUdpSocket = new QUdpSocket(this);

    //if (m_pUdpSocket->bind(QHostAddress::LocalHost, 9999)) {
    if (m_pUdpSocket->bind(QHostAddress("192.168.95.61"), 9999)) {
        qDebug() << "Bind udp socket success!";
        connect(m_pUdpSocket, SIGNAL(readyRead()), this, SLOT(slotReadUdpSocket()));
        connect(m_pUdpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotHandleUdpSocketError(QAbstractSocket::SocketError)));
    }
    if (m_pTcpSocket->bind(QHostAddress::LocalHost, 9998)) {
        qDebug() << "Bind tcp socket success!";
        connect(m_pTcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadTcpSocket()));
        connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotHandleTcpSocketError(QAbstractSocket::SocketError)));
    }
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_m_pButtonEncrypt_clicked()
{
    unsigned char buffer[65] = {0};
    QByteArray byteArray = ui->m_pEncryptIn->text().toUtf8();
#if 0
    byteArray.append('\07');
    byteArray.append('\07');
    byteArray.append('\07');
    byteArray.append('\07');
    byteArray.append('\07');
    byteArray.append('\07');
    byteArray.append('\07');
#endif
    qDebug() << "Encrypt src data:" << byteArray;
    if (0 == des_encryption((unsigned char *)byteArray.data(),
            DES_KEY, buffer)) {
        qDebug() << "Encrypt result:" << QString((char *)buffer);
        ui->m_pEncryptOut->setText(QString((char*)buffer));
    }
}

void Dialog::on_m_pButtonDecrypt_clicked()
{
    unsigned char buffer[65] = {0};
    if (0 == des_decryption((unsigned char *)ui->m_pDecryptIn->text().toUtf8().data(),
            DES_KEY, buffer)) {
        qDebug() << "Decrypt result:" << QString((char *)buffer);
        ui->m_pDecryptOut->setText(QString((char*)buffer));
    }
}

void Dialog::on_m_pButtonSend_clicked()
{
    if (ui->m_pComboBox->currentText() == "UDP") {
        qDebug() << "Start to send udp data:" << ui->m_pTextSend->text();
        m_pUdpSocket->writeDatagram(ui->m_pTextSend->text().toLocal8Bit(), QHostAddress::Broadcast, UDP_PORT);
    } else {
        if (m_pTcpSocket->state() != QAbstractSocket::ConnectedState) {
            m_pTcpSocket->connectToHost(ui->m_pServerIP->text(), TCP_PORT);
            if (!m_pTcpSocket->waitForConnected(5000)) {
                qDebug() << "Tcp connect failed!";
                return;
            }
        }
        qDebug() << "Tcp is connected!";
        qDebug() << "Start to send tcp data:" << ui->m_pTextSend->text();
        int len = m_pTcpSocket->write(ui->m_pTextSend->text().toLocal8Bit());
        qDebug() << "Send tcp data length:" << len;
    }
}

void Dialog::slotReadTcpSocket()
{
    QByteArray data = m_pTcpSocket->readAll();
    qDebug() << "Receive data from tcp socket:" << data;
    ui->m_pTextReceive->setText(data);
    m_pTcpSocket->close();
}

void Dialog::slotReadUdpSocket()
{
    QByteArray datagram;
    datagram.resize(m_pUdpSocket->pendingDatagramSize());
    m_pUdpSocket->readDatagram(datagram.data(), datagram.size());
    qDebug() << "Receive data from udp socket:" << datagram;
    ui->m_pTextReceive->setText(datagram);
}

void Dialog::slotHandleTcpSocketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    qDebug() << "Tcp socket error: " << m_pTcpSocket->errorString();
}

void Dialog::slotHandleUdpSocketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    qDebug() << "Udp socket error: " << m_pUdpSocket->errorString();
}

void Dialog::on_m_pButtonArpSend_clicked()
{
    ui->m_pTableIP->clear();
    QString strOut;
    for (int i = 1; i < 255; ++i) {
        QString strIP = ui->m_pLineIP->text() + QString("%1").arg(i);
        if (getMacByArp(strIP, strOut)) {
            int row = ui->m_pTableIP->rowCount();
            QTableWidgetItem *newItemIP = new QTableWidgetItem(strIP);
            QTableWidgetItem *newItemMac = new QTableWidgetItem(strOut);
            ui->m_pTableIP->insertRow(row);
            ui->m_pTableIP->setItem(row, 0, newItemIP);
            ui->m_pTableIP->setItem(row, 1, newItemMac);
        }
    }
    qDebug() << "Get ip mac finish!";
}

bool Dialog::getMacByArp(const QString strIP, QString &strOut)
{
    DWORD dwRetVal;
    IPAddr DestIp = 0;
    IPAddr SrcIp = 0;       /* default for src ip */
    ULONG MacAddr[2];       /* for 6-byte hardware addresses */
    ULONG PhysAddrLen = 6;  /* default to length of six bytes */

    const char *SrcIpString = NULL;//"192.168.1.140";
    //SrcIp = inet_addr(SrcIpString);
    int i = 0;

    BYTE *bPhysAddr;


    DestIp = inet_addr(strIP.toUtf8().data());
    strOut.clear();

    memset(&MacAddr, 0xff, sizeof (MacAddr));

    qDebug("Sending ARP request for IP address: (%ld)%s", DestIp, strIP.toUtf8().data());

    dwRetVal = SendARP(DestIp, SrcIp, &MacAddr, &PhysAddrLen);

    if (dwRetVal == NO_ERROR) {
        bPhysAddr = (BYTE *) & MacAddr;
        if (PhysAddrLen) {
            for (i = 0; i < (int) PhysAddrLen; i++) {
                if (i == (PhysAddrLen - 1)) {
                    strOut.append(QString::asprintf("%.2X ", (int) bPhysAddr[i]));
                    //qDebug("%.2X", (int) bPhysAddr[i]);
                } else {
                    strOut.append(QString::asprintf("%.2X:", (int) bPhysAddr[i]));
                    //qDebug("%.2X:", (int) bPhysAddr[i]);
                }
            }
            qDebug() << "Get its mac:" << strOut;
            return true;
        } else {
            qDebug("Warning: SendArp completed successfully, but returned length=0\n");
            return false;
        }
    } else {
        qDebug("Error: SendArp failed with error: %d", dwRetVal);
        switch (dwRetVal) {
        case ERROR_GEN_FAILURE:
            qDebug(" (ERROR_GEN_FAILURE)\n");
            break;
        case ERROR_INVALID_PARAMETER:
            qDebug(" (ERROR_INVALID_PARAMETER)\n");
            break;
        case ERROR_INVALID_USER_BUFFER:
            qDebug(" (ERROR_INVALID_USER_BUFFER)\n");
            break;
        case ERROR_BAD_NET_NAME:
            qDebug(" (ERROR_GEN_FAILURE)\n");
            break;
        case ERROR_BUFFER_OVERFLOW:
            qDebug(" (ERROR_BUFFER_OVERFLOW)\n");
            break;
        case ERROR_NOT_FOUND:
            qDebug(" (ERROR_NOT_FOUND)\n");
            break;
        default:
            qDebug("\n");
            break;
        }
    }
    return false;
}

void Dialog::on_m_pEditFilter_textChanged(const QString &arg1)
{
//    QList<QTableWidgetItem*> listItems = ui->m_pTableIP->findItems(arg1, Qt::MatchContains);
//    foreach (item, listItems) {
//        ui->m_pTableIP->showRow(ui->m_pTableIP->row(item));
//    }
    for (int i = 0; i < ui->m_pTableIP->rowCount(); ++i) {
        if (ui->m_pTableIP->item(i, 1)->text().contains(arg1, Qt::CaseInsensitive)) {
            ui->m_pTableIP->showRow(i);
        } else {
            ui->m_pTableIP->hideRow(i);
        }
    }
}
