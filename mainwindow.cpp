#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDataStream>
#include <QMessageBox>
#include <QTcpServer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    listen_port(1600)
{
    ui->setupUi(this);
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
        audthread = new AudioDataSocketThread(QString("/dev/dsp"));
        audthread->setLocalPort(1500);
        audthread->connectToHost(remote_ip, 1500);
        audthread->start();

        vidthread = new VideoDataSocketThread(QString("/dev/video"));
        vidthread->setLocalPort(1501);
        vidthread->connectToHost(remote_ip, 1501);
        vidthread->setDisplayWidget();
        vidthread->start();

        started_talking = true;

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

            remote_ip = tcp_socket->peerAddress();

            audthread = new AudioDataSocketThread(QString("/dev/dsp"));
            audthread->setLocalPort(1500);
            audthread->connectToHost(remote_ip, 1500);
            audthread->start();

            vidthread = new VideoDataSocketThread(QString("/dev/video"));
            vidthread->setLocalPort(1501);
            vidthread->connectToHost(remote_ip, 1501);
            vidthread->setDisplayWidget();
            vidthread->start();

            started_talking = true;

            ui->btn_dial->setEnabled(false);
            ui->btn_disconnect->setEnabled(true);
            ui->remote_ip->setText(remote_ip.toString());
            ui->remote_ip->setEnabled(false);
            ui->status->setText("Talking ...");
            ui->status->setEnabled(true);
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
    remote_ip = QHostAddress(ui->remote_ip->text());

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

        audthread->stop();
        delete audthread;

        vidthread->stop();
        delete vidthread;

        started_talking = false;
    }

    tcp_socket->abort();
    delete tcp_socket;

    ui->btn_dial->setEnabled(true);
    ui->btn_disconnect->setEnabled(false);
    ui->status->setVisible(false);
    ui->remote_ip->setText(remote_ip.toString());
    ui->remote_ip->setEnabled(true);
}

void MainWindow::display_socket_error(QAbstractSocket::SocketError socketError)
{
    QMessageBox::information(this, tr("Arm Phone"),
                             tr("The following error occurred: %1.")
                             .arg(tcp_socket->errorString()));
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
