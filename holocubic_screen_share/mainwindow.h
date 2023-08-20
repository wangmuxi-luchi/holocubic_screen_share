#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QBuffer>
#include <QPixmap>

#include "screenshot.h"
#include "tcpclient.h"
#include "ScreenWidget.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    quint8 select_byte=1;
    Screenshot *my_Screenshot;
    TCPclient *my_TCP_Client;
    ScreenWidget* my_getregion;
    QTimer *tim;
    QTime *counttim;
    int pre_tim;
    QString server_IP = "192.168.43.111";
    int gTcpPort=8081;
    int connect_flag = 0;
    int send_flag = 0;
    int check_connect_count=0;


    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    int img_w=240;
    int img_h=240;
    QPixmap dis_pixmap;


private slots:
    void update_img();
    void on_connectBtn_clicked();
    void on_disconnectBtn_clicked();
    void ReadData();
    void check_connect();
    void set_dispregion();

    void on_selectregionBtn_clicked();
    void on_img_w_lineEdit_editingFinished();
    void on_img_h_lineEdit_editingFinished();
};

#endif // MAINWINDOW_H
