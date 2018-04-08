#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QUdpSocket>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_m_pButtonEncrypt_clicked();

    void on_m_pButtonDecrypt_clicked();

    void on_m_pButtonSend_clicked();
    void slotReadTcpSocket();
    void slotReadUdpSocket();
    void slotHandleTcpSocketError(QAbstractSocket::SocketError socketError);
    void slotHandleUdpSocketError(QAbstractSocket::SocketError socketError);

private:
    Ui::Dialog *ui;
    QTcpSocket *m_pTcpSocket;
    QUdpSocket *m_pUdpSocket;
};

#endif // MAINDIALOG_H
