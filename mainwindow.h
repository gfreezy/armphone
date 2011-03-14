#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui {
    class MainWindow;
}

class AudioDevice;
class UdpSocket;
class NetPlayThread;
class NetRecThread;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void start_talking();
    void stop_talking();
    Ui::MainWindow *ui;
    QTcpSocket* tcp_socket;
    QString remote_ip;
    QString remote_port;
    bool is_connected;
    AudioDevice* aud;
    UdpSocket* talking_socket;
    NetRecThread* netrec_thread;
    NetPlayThread* netplay_thread;

private slots:
    void on_btn_disconnect_clicked();
    void on_btn_dial_clicked();
    void ready_to_read();
    void display_socket_error(QAbstractSocket::SocketError);
};

#endif // MAINWINDOW_H
