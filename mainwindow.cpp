#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "audiodevice.h"
#include "udpsocket.h"
#include "netplaythread.h"
#include "netrecthread.h"
#include <QDataStream>
#include <QMessageBox>
#include <QTcpServer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    remote_ip = "";
    remote_port = "1600";
    ui->status->setVisible(false);
    is_connected = false;

    tcp_server = new QTcpServer();
    tcp_server->listen(QHostAddress::Any, remote_port.toInt());
    connect(tcp_server, SIGNAL(newConnection()), this, SLOT(new_connection()));
}

MainWindow::~MainWindow()
{
    delete tcp_server;
    delete tcp_socket;
    delete ui;
}

void MainWindow::ready_to_read()
{
    QDataStream in(tcp_socket);
    QString recv_data;
    in >> recv_data;
    if(recv_data == "accept")
    {
        start_talking();
        ui->btn_disconnect->setEnabled(true);
        ui->status->setText("Talking ...");
        is_connected = true;
    }
    else if(recv_data == "stop")
    {
        stop_talking();
        ui->btn_dial->setEnabled(true);
        ui->btn_disconnect->setEnabled(false);
        ui->status->setVisible(false);
        ui->remote_ip->setEnabled(true);
        ui->remote_port->setEnabled(true);
        is_connected = false;
    }
    else if(recv_data == "dial request")
    {
        int res = QMessageBox::question(this, tr("Accept?"), tr("Accept the dial request"),
                              QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
        if(res == QMessageBox::Yes)
        {
            in << "accpet";

        }
        else if(res == QMessageBox::No)
        {
            in << "reject";
        }
    }
    else if(recv_data == "stop")
    {
        on_btn_disconnect_clicked();
    }

}

void MainWindow::on_btn_dial_clicked()
{
    tcp_socket = new QTcpSocket();
    connect(tcp_socket, SIGNAL(readyRead()), this, SLOT(ready_to_read()));
    connect(tcp_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(display_socket_error(QAbstractSocket::SocketError)));
    ui->btn_dial->setEnabled(false);
    ui->btn_disconnect->setEnabled(true);
    ui->remote_ip->setEnabled(false);
    ui->remote_port->setEnabled(false);
    remote_ip = ui->remote_ip->text();
    remote_port = ui->remote_port->text();
    tcp_socket->abort();
    tcp_socket->connectToHost(remote_ip, remote_port.toInt());
    if(tcp_socket->waitForConnected())
    {
        QDataStream out(tcp_socket);
        out << "dial request";
        ui->status->setText("Waitting For Reply");
        ui->status->setVisible(true);
    }
    else
    {
        on_btn_disconnect_clicked();
    }

}

void MainWindow::on_btn_disconnect_clicked()
{
    if(is_connected)
    {
        QDataStream out(tcp_socket);
        out << "stop";
        stop_talking();
    }
    tcp_socket->abort();
    ui->btn_dial->setEnabled(true);
    ui->btn_disconnect->setEnabled(false);
    ui->status->setVisible(false);
    ui->remote_ip->setEnabled(true);
    ui->remote_port->setEnabled(true);
}

void MainWindow::start_talking()
{
    aud = new AudioDevice();
    if(-1 == aud->open_device())
        return;
    if(-1 == aud->init())
    {
        aud->close_device();
        return;
    }

    talking_socket = new UdpSocket();

    talking_socket->bind_(1500);
    talking_socket->connect_(remote_ip.toStdString().c_str(), 1500);

    netrec_thread = new NetRecThread(aud, talking_socket);
    netplay_thread = new NetPlayThread(aud, talking_socket);

    netrec_thread->start();
    netplay_thread->start();
}

void MainWindow::stop_talking()
{
    netrec_thread->stop_record();
    netplay_thread->stop_play();

    talking_socket->close_();
    aud->close_device();
    delete aud;
    delete talking_socket;
    delete netplay_thread;
    delete netrec_thread;
}

void MainWindow::display_socket_error(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Arm Phone"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        on_btn_disconnect_clicked();
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Arm Phone"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        on_btn_disconnect_clicked();
        break;
    default:
        QMessageBox::information(this, tr("Arm Phone"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcp_socket->errorString()));
        on_btn_disconnect_clicked();
    }
}

void MainWindow::new_connection()
{
    tcp_socket = tcp_server->nextPendingConnection();
    connect(tcp_socket, SIGNAL(readyRead()), this, SLOT(ready_to_read()));
    connect(tcp_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(display_socket_error(QAbstractSocket::SocketError)));
}














