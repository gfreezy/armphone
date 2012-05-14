#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include "audiodatasocketthread.h"
#include "videodatasocketthread.h"
#include "videowidget.h"
namespace Ui {
    class Dialog;
}

class AudioDevice;
class UdpSocket;
class NetPlayThread;
class NetRecThread;
class QTcpServer;
class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
    QTcpSocket* tcp_socket;
    QHostAddress remote_ip;
    bool started_talking;
    QTcpServer* tcp_server;
    const int listen_port;
    AudioDataSocketThread* audthread;
    VideoDataSocketThread* vidthread;
    VideoWidget *vd_widget;

private slots:
    void on_btn_disconnect_clicked();
    void on_btn_dial_clicked();
    void ready_to_read();
    void display_socket_error(QAbstractSocket::SocketError);
    void new_connection();
};

#endif // DIALOG_H
