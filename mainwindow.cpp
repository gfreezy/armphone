#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "audiodevice.h"
#include "udpsocket.h"
#include "netplaythread.h"
#include "netrecthread.h"
#include <QDataStream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    remote_ip = "";
    remote_port = "1500";
    tcp_socket = new QTcpSocket();
    ui->status->setVisible(false);
    connect(tcp_socket, SIGNAL(readyRead()), this, SLOT(ready_to_read()));
    connect(tcp_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(display_socket_error(QAbstractSocket::SocketError)));
    is_connected = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ready_to_read()
{
    QDataStream in(tcp_socket);
    QString reply;
    in >> reply;
    if(reply == "accept")
    {
        start_talking();
        ui->btn_disconnect->setEnabled(true);
        ui->status->setText("Talking ...");
        is_connected = true;
    }
    else if(reply == "stop")
    {
        stop_talking();
        ui->btn_dial->setEnabled(true);
        ui->btn_disconnect->setEnabled(false);
        ui->status->setVisible(false);
        ui->remote_ip->setEnabled(true);
        ui->remote_port->setEnabled(true);
        is_connected = false;
    }
    else
    {
        return;
    }

}

void MainWindow::on_btn_dial_clicked()
{
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
















