#include "videodatasocket.h"
#include <QDateTime>

VideoDataSocket::VideoDataSocket(const QString& dev, QObject *parent)
    :DataSocket(parent), dev(dev)
{
    stop_sending = false;

    connect(this, SIGNAL(readyRead()), this, SLOT(ready_read()));

    viddev = new VideoDevice(dev);

    connect(viddev, SIGNAL(display_error(QString)), this, SLOT(display_error(QString)));
    if(-1 == viddev->open_device())
    {
        exit(EXIT_FAILURE);
    }

    if(-1 == viddev->init_device())
    {
        exit(EXIT_FAILURE);
    }

    send_buf = new FrameData();
    recv_buf = new FrameData();
}

void VideoDataSocket::setDisplayWidget(VideoWidget *widget)
{
    this->vw = widget;
}

void VideoDataSocket::ready_read()
{
    readDatagram((char *)recv_buf, sizeof(FrameData));

//    char buf[1024];
//    quint64 len;
//    decode(recv_buf, recv_len, buf, &len);                      //decode

    qDebug("recv pic");
    updateDisplayWidget(recv_buf);
}

void VideoDataSocket::updateDisplayWidget(FrameData *buf)
{
    vw->updateImage((uchar *)buf, buf->len);
}

void VideoDataSocket::sendData()
{
    if(-1 == viddev->start_capturing())
    {
        exit(EXIT_FAILURE);
    }

    char* frame;
    size_t frame_len;

//    char rgb_frame[1024];
//    quint64 rgb_frame_len;

//    char buf[1024];            //encoded buf
//    quint64 len;

    forever
    {

        if(-1 == viddev->get_frame((void**)&frame, &frame_len))
        {
            exit(EXIT_FAILURE);
        }

        qDebug("send pict");
//        yuyvToRGB(frame, frame_len, rgb_frame, &rgb_frame_len);

//        encode(rgb_frame, rgb_frame_len, buf, &len);

        quint8 *ptdeb, *ptcur;
        ptdeb = ptcur = (quint8 *)frame;
        while (((ptcur[0] << 8) | ptcur[1]) != 0xffc0)
            ptcur++;
        quint64 sizein = ptcur-ptdeb;
        if(sizein != 175)
        {
            qDebug("sizein is not 175");
            return;
        }
        memcpy(send_buf, frame, sizein);
        memcpy(send_buf->dht, dht_data, DHT_SIZE);
        memcpy(send_buf->pic_data, ptcur, frame_len - sizein);
        send_buf->len = frame_len + DHT_SIZE;
        send_buf->timestamp = QDateTime::currentDateTime().toTime_t();

        writeDatagram((char *)send_buf, sizeof(FrameData), *addr, port);

        if(-1 == viddev->unget_frame())
        {
            exit(EXIT_FAILURE);
        }

        if(stop_sending)
        {
            break;
        }
        sleep(1);
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
    delete recv_buf;
    delete send_buf;
    viddev->uninit_device();
    viddev->close_device();
    delete viddev;

}

void VideoDataSocket::display_error(QString err)
{
    qDebug(err.toStdString().c_str());
}
