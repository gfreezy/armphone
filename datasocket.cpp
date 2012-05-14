#include "datasocket.h"

DataSocket::DataSocket(QObject *parent) :
    QUdpSocket(parent)
{
    stop_sending = false;
    stopped = false;
}

void DataSocket::bindPort(quint16 port)
{
    setLocalPort(port);
    bind(QHostAddress::Any, port);
}

void DataSocket::destroy()
{
    abort();
}

void DataSocket::setRemoteHost(QHostAddress& addr, quint16 port)
{
    this->addr = new QHostAddress(addr);
    this->port = port;
}

void DataSocket::stopSendingData()
{
    stop_sending = true;
    while(!stopped)
    {
        sleep(1);
    }
}

DataSocket::~DataSocket()
{
    delete addr;
}

