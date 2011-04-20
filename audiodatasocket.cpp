#include "audiodatasocket.h"

AudioDataSocket::AudioDataSocket(const QString &aud, QObject *parent)
    :DataSocket(parent), aud(aud)
{
    stop_sending = false;

    connect(this, SIGNAL(readyRead()), this, SLOT(ready_read()));

    auddev = new AudioDevice(aud);
    if((-1 == auddev->open_device()) || (-1 == auddev->init_device()))
    {
        exit(EXIT_FAILURE);
    }
}

void AudioDataSocket::ready_read()
{
    fprintf(stderr, "receving data\n");
    char buf[512];
    quint64 len = read(buf, sizeof(buf));
    auddev->write_data((short*)buf, len);
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
        write(buf, sizeof(buf));
        if(stop_sending)
        {
           break;
        }
//        fprintf(stderr, "sending data\n");
    }
    stopped = true;
}

