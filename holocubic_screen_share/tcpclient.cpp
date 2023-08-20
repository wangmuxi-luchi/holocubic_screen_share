#include "tcpclient.h"

TCPclient::TCPclient(QWidget *parent) : QWidget(parent)
{
    //初始化TCP客户端
    tcpClient = new QTcpSocket(this);   //实例化tcpClient
    tcpClient->abort();                 //取消原有连接
}

//void TCPclient::ReadData()
//{
//    QByteArray bt;
//    bt.resize(tcpClient->bytesAvailable());
//    tcpClient->read(bt.data(),bt.size());
//    //将客户端反馈的数据显示到标签上
//    qDebug()<<"Socketrecv:"<<bt<<endl;
//}

void TCPclient::ReadError(QAbstractSocket::SocketError e)
{
    qDebug()<<"SocketError:"<<e<<endl
           <<tcpClient->errorString();
    tcpClient->disconnectFromHost();
}

void TCPclient::connect(QString IP,QString port)
{
    tcpClient->abort();
    //连接服务端
    tcpClient->connectToHost(IP,port.toInt());
}
