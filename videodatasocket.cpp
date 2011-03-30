#include "videodatasocket.h"

VideoDataSocket::VideoDataSocket(const QString& dev, QObject *parent)
    :DataSocket(parent), dev(dev)
{
    stop_sending = false;

    connect(this, SIGNAL(readyRead()), this, SLOT(ready_read()));

    viddev = new VideoDevice(dev);
    if((-1 == viddev->open_device()) || (-1 == viddev->init_device()))
    {
        exit(EXIT_FAILURE);
    }
}

void VideoDataSocket::setDisplayWidget()
{

}

void VideoDataSocket::ready_read()
{
    char recv_buf[1024];                //set receive buf size
    quint64 recv_len = read(recv_buf, sizeof(recv_buf));

    char buf[1024];
    quint64 len;
    decode(recv_buf, recv_len, buf, &len);                      //decode

    updateDisplayWidget(buf, len);
}

void VideoDataSocket::updateDisplayWidget(char *buf, quint64 len)
{

}

void VideoDataSocket::sendData()
{
    if(-1 == viddev->start_capturing())
    {
        exit(EXIT_FAILURE);
    }

    char* frame;
    size_t frame_len;

    char rgb_frame[1024];
    quint64 rgb_frame_len;

    char buf[1024];            //encoded buf
    quint64 len;

    forever
    {
        if(-1 == viddev->get_frame(frame, &frame_len))
        {
            exit(EXIT_FAILURE);
        }

        yuyvToRGB(frame, frame_len, rgb_frame, &rgb_frame_len);

        encode(rgb_frame, rgb_frame_len, buf, &len);

        write(buf, len);

        if(-1 == viddev->unget_frame())
        {
            exit(EXIT_FAILURE);
        }

        if(stop_sending)
        {
            break;
        }
    }

    if(-1 == viddev->stop_capturing())
    {
        exit(EXIT_FAILURE);
    }

    stopped = true;
}

void VideoDataSocket::encode(char *buf1, quint64 len1, char *buf2, quint64 *len2)
{

}

void VideoDataSocket::decode(char *buf1, quint64 len1, char *buf2, quint64 *len2)
{

}

void VideoDataSocket::yuyvToRGB(char *buf1, quint64 len1, char *buf2, quint64 *len2)
{

}

VideoDataSocket::~VideoDataSocket()
{
    viddev->uninit_device();
    viddev->close_device();
    delete viddev;

}
