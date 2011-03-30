#include "audiodatasocketthread.h"

AudioDataSocketThread::AudioDataSocketThread(const QString &aud, QObject *parent) :
    QThread(parent), aud(aud)
{
    audsock = new AudioDataSocket(aud);
    localport = 6000;
}

void AudioDataSocketThread::run()
{

    audsock->sendData();                   //sending data forever
}

void AudioDataSocketThread::setLocalPort(quint16 port)
{
    localport = port;
}

void AudioDataSocketThread::connectToHost(QHostAddress &addr, quint16 port)
{
    audsock->create(localport);
    audsock->connectToHost(addr, port);
}

void AudioDataSocketThread::stop()
{
    audsock->stopSendingData();
    audsock->destroy();
    delete audsock;
}


