#include "videodatasocketthread.h"

VideoDataSocketThread::VideoDataSocketThread(const QString dev, QObject *parent) :
    QThread(parent), dev(dev)
{
    vidsock = new VideoDataSocket(dev);
}

void VideoDataSocketThread::connectToHost(QHostAddress &addr, quint16 port)
{
    vidsock->bindPort(port);
    vidsock->setRemoteHost(addr, port);
}

void VideoDataSocketThread::stop()
{
    vidsock->stopSendingData();
    vidsock->destroy();
    delete vidsock;
}

void VideoDataSocketThread::setDisplayWidget(VideoWidget *vw)
{
    vidsock->setDisplayWidget(vw);
}

void VideoDataSocketThread::run()
{
    vidsock->sendData();
}
