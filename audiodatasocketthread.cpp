#include "audiodatasocketthread.h"

AudioDataSocketThread::AudioDataSocketThread(const QString &aud, QObject *parent) :
    QThread(parent), aud(aud)
{
    audsock = new AudioDataSocket(aud);
}

void AudioDataSocketThread::run()
{
    audsock->sendData();                   //sending data forever
}

void AudioDataSocketThread::connectToHost(QHostAddress &addr, quint16 port)
{
    audsock->bindPort(port);
    audsock->setRemoteHost(addr, port);
}

void AudioDataSocketThread::stop()
{
    audsock->stopSendingData();
    audsock->destroy();
    delete audsock;
}


