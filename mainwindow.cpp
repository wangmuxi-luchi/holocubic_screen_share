#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    my_getregion = ScreenWidget::Instance();
    my_Screenshot = new Screenshot();//屏幕内容获取
    tim = new QTimer();             //定时器，定时检测连接状态，触发自动重连，类似看门狗
    counttim = new QTime();         //获取时间，用于计算帧率
    my_TCP_Client = new TCPclient;  //tcp连接

    tim->setInterval(100);
    connect(tim,SIGNAL(timeout()),this,SLOT(check_connect()));

    //设置客户端连接默认的主机地址及端口号
    ui->hostLineEdit->setText("192.168.43.111");
    ui->portLineEdit->setText(QString::number(gTcpPort));
    ui->img_w_lineEdit->setText("240");
    ui->img_h_lineEdit->setText("240");
    connect(my_TCP_Client->tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));//接收到信号
    connect(my_TCP_Client->tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), \
            my_TCP_Client, SLOT(ReadError(QAbstractSocket::SocketError)));

    //获取投屏范围
    connect(my_getregion,SIGNAL(get_newregion()),this,SLOT(set_dispregion()));
//    tim->start();
}

MainWindow::~MainWindow()
{
    on_disconnectBtn_clicked();
    delete my_Screenshot;//屏幕内容获取
    delete tim ;             //定时器，定时检测连接状态，触发自动重连，类似看门狗
    delete counttim ;         //获取时间，用于计算帧率
    delete my_TCP_Client ;  //tcp连接
    delete my_getregion;
    delete ui;
}
void MainWindow::update_img()
{
//    qDebug()<<"update_img\n";
    my_Screenshot->shootScreen();
//    dis_pixmap = my_Screenshot->originalPixmap.copy(x, y, w, h);
    ui->screenshotLabel->setPixmap(my_Screenshot->originalPixmap.copy(x, y, w, h).scaled(ui->screenshotLabel->size() ,
                                Qt::KeepAspectRatio,Qt::SmoothTransformation));
    if(connect_flag==1)
    {
        QByteArray bytes1;
        QBuffer buffer((&bytes1));
        buffer.open(QIODevice::ReadWrite);
//        QPixmap scaled_img = my_Screenshot->originalPixmap.copy(x, y, w, h).scaled(img_w,img_h);
        QPixmap scaled_img = ui->screenshotLabel->pixmap()->scaled(img_w,img_h);
        scaled_img.save(&buffer, "jpeg"); // 转换为jpeg格式，存到buffer中

//        quint16 u16size;
//        QByteArray tembyte;

//        tembyte.resize(3);
//        tembyte[0] = 0xaa; tembyte[1] = 0xbb; tembyte[2] = 0xcc;
//        bytes1.append(tembyte);

//        u16size = bytes1.size();
//        tembyte.resize(2);
//        //转换 int[] -> QByteArray
//        memcpy(tembyte.data(), &u16size, 2);

//        bytes1.prepend(tembyte);
        my_TCP_Client->tcpClient->write(bytes1);
        counttim->restart();
        QString fps_dis = "帧率：" + QString::number(1000.0/((1000+counttim->msec()-pre_tim)%1000),'f',2) + "fps";
        qDebug() << fps_dis;
        ui->fps_Label->setText(fps_dis);
        pre_tim = counttim->msec();
        send_flag = 0;
        check_connect_count = 0;

    }

}

void MainWindow::check_connect()
{

    check_connect_count++;
    if(check_connect_count>50)
    {
        check_connect_count=0;
        qDebug()<<"reconnect"<<endl;
        on_disconnectBtn_clicked();
        QTimer::singleShot(500, this, SLOT(on_connectBtn_clicked()));  // SLOT槽填入一个槽函数
    }
}

void MainWindow::ReadData()
{
    QByteArray bt;
    bt.resize(my_TCP_Client->tcpClient->bytesAvailable());
    my_TCP_Client->tcpClient->read(bt.data(),bt.size());
    //将客户端反馈的数据显示到标签上
//    qDebug()<<"Socketrecv:"<<bt<<endl;
    for(int i=0;i<bt.size()-1;i+=2)
    {
        if(bt[i]=='o'&&bt[i+1]=='k')
        {
//            qDebug()<<"ookk"<<endl;
            send_flag=1;
        }
        else send_flag=0;
    }
    if(send_flag)update_img();
}

void MainWindow::set_dispregion()
{
    x = my_getregion->screen->getLeftUp().x();
    y = my_getregion->screen->getLeftUp().y();
    w = my_getregion->screen->getRightDown().x() - x;
    h = my_getregion->screen->getRightDown().y() - y;
    my_getregion->close();

    qDebug()<<"x"<<x<<" y"<<y<<" w"<<w<<" h"<<h<<endl;
}

void MainWindow::on_connectBtn_clicked()
{
    if(connect_flag==0)
    {
        qDebug()<<"start connect"<<endl;
        my_TCP_Client->connect(ui->hostLineEdit->text(),ui->portLineEdit->text());
        connect_flag = 1;
        tim->start();//打开定时连接检测
        check_connect_count=0;
    }
}

void MainWindow::on_disconnectBtn_clicked()
{
    if(connect_flag==1)
    {
        qDebug()<<"stop connect"<<endl;
        my_TCP_Client->tcpClient->abort();
        connect_flag = 0;
        tim->stop();//关闭定时连接检测
    }

}

void MainWindow::on_selectregionBtn_clicked()
{

    my_getregion->showFullScreen();   //直接调用实例
}

void MainWindow::on_img_w_lineEdit_editingFinished()
{
    int w = ui->img_w_lineEdit->text().toInt();
    if(w>0&&w<2000)img_w=w;
}

void MainWindow::on_img_h_lineEdit_editingFinished()
{
    int h = ui->img_h_lineEdit->text().toInt();
    if(h>0&&h<2000)img_h=h;
}
