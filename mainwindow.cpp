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
    ui(new Ui::MainWindow),
    listen_port(1600)
{
    ui->setupUi(this);
    remote_ip = "";
    ui->status->setVisible(false);
    started_talking = false;
    tcp_server = new QTcpServer(this);
    tcp_server->listen(QHostAddress::Any, listen_port);
    connect(tcp_server, SIGNAL(newConnection()), this, SLOT(new_connection()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ready_to_read()
{
    QDataStream in(tcp_socket);
    char* data;
    in >> data;
    QString recv_data(data);
//    QMessageBox::information(this, tr("new signal"),
//                              tr("received signal:\n %1").arg(recv_data));
    if(recv_data == "accept")
    {
//        QMessageBox::information(this, tr("accept"), tr("accept"));
        start_talking();
        ui->btn_disconnect->setEnabled(true);
        ui->status->setText("Talking ...");
    }
    else if(recv_data == "dial request")
    {
        int res = QMessageBox::question(this, tr("Accept?"), tr("Accept the dial request"),
                              QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
        if(res == QMessageBox::Yes)
        {
            in << "accept";
            remote_ip = tcp_socket->peerAddress().toString();
            start_talking();
            ui->btn_dial->setEnabled(false);
            ui->btn_disconnect->setEnabled(true);
            ui->remote_ip->setText(remote_ip);
            ui->remote_ip->setEnabled(false);
            ui->status->setText("Talking ...");

        }
        else if(res == QMessageBox::No)
        {
            in << "reject";
            QMessageBox::information(this, tr("Decline"), tr("Your request is declined"));
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
    remote_ip = ui->remote_ip->text();
    tcp_socket->connectToHost(remote_ip, listen_port);
    if(tcp_socket->waitForConnected())
    {
        QDataStream out(tcp_socket);
        out << "dial request";
        ui->status->setText("Waitting For Reply");
        ui->status->setVisible(true);
//        QMessageBox::information(this, tr("connection"),
//                                 tr("connected to server"));
    }
    else
    {
        on_btn_disconnect_clicked();
    }

}

void MainWindow::on_btn_disconnect_clicked()
{
    if(started_talking)
    {
        QDataStream out(tcp_socket);
        out << "stop";
        stop_talking();
    }
    tcp_socket->disconnectFromHost();
    if(tcp_socket->waitForDisconnected())
        tcp_socket->close();
    delete tcp_socket;
    ui->btn_dial->setEnabled(true);
    ui->btn_disconnect->setEnabled(false);
    ui->status->setVisible(false);

    remote_ip = "";
    ui->remote_ip->setText(remote_ip);
    ui->remote_ip->setEnabled(true);
}

void MainWindow::start_talking()
{
    started_talking = true;
    aud = new AudioDevice("/dev/dsp");
    if(-1 == aud->open_device())
        return;
    if(-1 == aud->init())
    {
        aud->close_device();
        return;
    }
//    QMessageBox::information(this, tr("open device"), tr("open device"));

    talking_socket = new UdpSocket();

    talking_socket->bind_(1500);
    talking_socket->connect_(remote_ip.toStdString().c_str(), 1500);

    netplay_thread = new NetPlayThread(aud, talking_socket);
    netrec_thread = new NetRecThread(aud, talking_socket);


    netrec_thread->start();
    netplay_thread->start();
}

void MainWindow::stop_talking()
{
    started_talking = false;
    netrec_thread->stop_record();
    netplay_thread->stop_play();
    netrec_thread->wait();
    netplay_thread->wait();

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
//        on_btn_disconnect_clicked();
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Arm Phone"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
//        on_btn_disconnect_clicked();
        break;
    default:
        QMessageBox::information(this, tr("Arm Phone"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcp_socket->errorString()));
//        on_btn_disconnect_clicked();
    }
}

void MainWindow::new_connection()
{
    tcp_socket = tcp_server->nextPendingConnection();
    connect(tcp_socket, SIGNAL(readyRead()), this, SLOT(ready_to_read()));
    connect(tcp_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(display_socket_error(QAbstractSocket::SocketError)));
//    QMessageBox::information(this, tr("new connection"),
//                             tr("accept new connection"));
}














