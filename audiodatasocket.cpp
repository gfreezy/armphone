#include "audiodatasocket.h"

AudioDataSocket::AudioDataSocket(const QString &aud, QObject *parent)
    :DataSocket(parent), aud(aud)
{
    stop_sending = false;

    connect(this, SIGNAL(readyRead()), this, SLOT(ready_read()));

    auddev = new AudioDevice(aud);
    if((-1 == auddev->open_device()) || (-1 == auddev->init_device()))
    {
        perror("open device");
        exit(EXIT_FAILURE);
    }
//    emit readyRead();
}

void AudioDataSocket::ready_read()
{
//    fprintf(stderr, "receving data from %d to %d\n", peerPort(), localPort());
    char buf[1024];
    if(hasPendingDatagrams())
    {
        quint64 len = pendingDatagramSize();
        if(len > sizeof(buf))
        {
            qDebug("out of buffer");
            exit(EXIT_FAILURE);
        }
        readDatagram(buf, len);
        auddev->write_data((short*)buf, len);
    }
}

AudioDataSocket::~AudioDataSocket()
{
    auddev->close_device();
    delete auddev;
}

void AudioDataSocket::sendData()
{

    char buf[512];
    forever
    {
        auddev->read_data((short*)buf, sizeof(buf));
        writeDatagram(buf, sizeof(buf), *addr, port);
        if(stop_sending)
        {
           break;
        }
//        fprintf(stderr, "send to port %d from %d", peerPort(), localPort());
    }
    stopped = true;
}

