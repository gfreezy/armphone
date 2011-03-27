#include "netplaythread.h"
#include "audiodevice.h"
#include "udpsocket.h"
#include "sys/poll.h"
#define RECEIVE_TIMEOUT 100
//extern "C"
//{
//    #include "gsm.h"
//}

NetPlayThread::NetPlayThread(AudioDevice *dev, UdpSocket *s, QObject *parent) :
    QThread(parent)
{
    this->aud_dev = dev;
    this->socket = s;
    is_stop = false;
    printf("netplaythread init");
}

void NetPlayThread::run()
{
    int res;
    //gsm g;

//    g = gsm_create();

    while (!is_stop)
    {
//        fprintf(stderr,"is_stop");
//        int i;
//        unsigned char packet[65536];
//        int numframes;
        short buf[256];
        struct pollfd pfd;

        pfd.fd = socket->get_socket_fd();
        pfd.events = POLLIN;
        res = poll(&pfd, 1, RECEIVE_TIMEOUT);
        if (res == -1)
            perror("Netplay: poll error");
        if (res == 0)
        {
            /* res = aud_write(ad, NULL, 0);
                            if (res == -1)
                                    err_quit("Netplay: writing error");
                            */
            continue;
        }
        else
        {
            res = socket->read_(buf, sizeof(buf));
//            numframes = res /33;

//            for(i = 0; i < numframes; i++)
//            {
//                short buf[160];

                //res = gsm_decode(g, packet + 33 * i, buf);
                //if(res != 0)
//                    perror("Error decoding gsm data\n");

            res = aud_dev->write_data(buf, sizeof(buf));
            if(res == -1)
                perror("Netplay: writing error");
//            }
        }
    }

    return;

}

void NetPlayThread::stop_play()
{
    is_stop = true;
}





