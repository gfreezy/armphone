#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

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
    void start_talking();
    void stop_talking();
    Ui::MainWindow *ui;
    QTcpSocket* tcp_socket;
    QString remote_ip;
    QString remote_port;
    bool is_connected;

private slots:
    void on_btn_disconnect_clicked();
    void on_btn_dial_clicked();
    void ready_to_read();
    void display_socket_error(QAbstractSocket::SocketError);
};

#endif // MAINWINDOW_H
