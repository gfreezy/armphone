#include "datasocket.h"

DataSocket::DataSocket(QObject *parent) :
    QUdpSocket(parent)
{
    stop_sending = false;
    stopped = false;
}

void DataSocket::create(quint16 port)
{
    setLocalPort(port);
}

void DataSocket::destroy()
{
    abort();
}

void DataSocket::stopSendingData()
{
    stop_sending = true;
    while(!stopped)
    {
        sleep(0.01);
    }
}

