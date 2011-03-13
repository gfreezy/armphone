#include "netplaythread.h"
#include "netrecthread.h"
#include "audiodevice.h"
#include "udpsocket.h"
#include <string>

int main(int argc, char *argv[])
{
    AudioDevice* aud = new AudioDevice;
    if(-1 == aud->open_device())
        return -1;
    if(-1 == aud->init())
        return -1;

    std::string host = "127.0.0.1";
    UdpSocket* s1 = new UdpSocket();
    s1->bind_(1500);
    s1->connect_(host.c_str(), 1600);

    UdpSocket* s2 = new UdpSocket();
    s2->bind_(1600);
    s2->connect_(host.c_str(), 1500);

    NetRecThread* netrec = new NetRecThread(aud, s1);
    NetPlayThread* netplay = new NetPlayThread(aud, s2);

    netrec->start();
    netplay->start();


    int i = 1;
    while(i < 10)
    {
        fprintf(stderr,"%d\n",i);
        sleep(1);
        i++;
    }


    netrec->stop_record();
    netplay->stop_play();
    netrec->wait();
    netplay->wait();
    s1->close_();
    s2->close_();
    aud->close_device();
    return 0;
}

