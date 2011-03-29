#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "audiodatasocketthread.h"

namespace Ui {
    class MainWindow;
}

class AudioDevice;
class UdpSocket;
class NetPlayThread;
class NetRecThread;
class QTcpServer;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpSocket* tcp_socket;
    QHostAddress remote_ip;
    bool started_talking;
    QTcpServer* tcp_server;
    const int listen_port;
    AudioDataSocketThread* audthread;

private slots:
    void on_btn_disconnect_clicked();
    void on_btn_dial_clicked();
    void ready_to_read();
    void display_socket_error(QAbstractSocket::SocketError);
    void new_connection();
};

#endif // MAINWINDOW_H
