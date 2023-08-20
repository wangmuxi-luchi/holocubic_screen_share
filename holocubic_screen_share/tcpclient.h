#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QAbstractSocket>

class TCPclient : public QWidget
{
    Q_OBJECT
public:
    explicit TCPclient(QWidget *parent = nullptr);

signals:

public slots:


private slots:
    //客户端槽函数
//    void ReadData();
    void ReadError(QAbstractSocket::SocketError);

public:
    void connect(QString IP,QString port);
    QTcpSocket *tcpClient;
};

#endif // TCPCLIENT_H
