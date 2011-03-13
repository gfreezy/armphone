#include "netrecthread.h"
#include "audiodevice.h"
#include "udpsocket.h"
extern "C"
{
    #include "gsm.h"
}


NetRecThread::NetRecThread(AudioDevice* dev, UdpSocket* socket, QObject *parent) :
    QThread(parent)
{
    this->aud_dev = dev;
    this->socket = socket;
    is_stop = false;
}

void NetRecThread::run()
{

    fprintf(stderr,"in run");
    int res;
    gsm g;
    g = gsm_create();

    while (!is_stop)
    {
        int i;
        unsigned char packet[65536];
        const int numframes = 4;

        for (i = 0; i < numframes; i++)
        {
            short buf[160];
            res = aud_dev->read_data(buf, sizeof(buf));
            if (res == -1)
                perror("Netrec: writing error");

            gsm_encode(g, buf, packet + 33*i);
        }

        socket->write_(packet, numframes * 33);
    }

    return;

}

void NetRecThread::stop_record()
{
    is_stop = true;
}
