#include "mainDialog.h"
#include "ui_mainDialog.h"

#include "jw_des.h"
#include <QtDebug>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>

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
    qDebug() << "Tcp socket error: " << m_pTcpSocket->errorString();
}

void Dialog::slotHandleUdpSocketError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Udp socket error: " << m_pUdpSocket->errorString();
}
