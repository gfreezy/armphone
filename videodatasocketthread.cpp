#include "videodatasocketthread.h"

VideoDataSocketThread::VideoDataSocketThread(const QString dev, QObject *parent) :
    QThread(parent), dev(dev)
{
    vidsock = new VideoDataSocket(dev);
    localport = 6001;
}

void VideoDataSocketThread::setLocalPort(quint16 port)
{
    localport = port;
}

void VideoDataSocketThread::connectToHost(QHostAddress &addr, quint16 port)
{
    vidsock->create(localport);
    vidsock->connectToHost(addr, port);
}

void VideoDataSocketThread::stop()
{
    vidsock->stopSendingData();
    vidsock->destroy();
    delete vidsock;
}

void VideoDataSocketThread::setDisplayWidget()
{
    vidsock->setDisplayWidget();
}

void VideoDataSocketThread::run()
{
    vidsock->sendData();
}