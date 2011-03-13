#ifndef NETPLAYTHREAD_H
#define NETPLAYTHREAD_H

#include <QThread>

class AudioDevice;
class UdpSocket;
class NetPlayThread : public QThread
{
    Q_OBJECT
public:
    explicit NetPlayThread(AudioDevice*, UdpSocket*, QObject *parent = 0);
    void run();
    void stop_play();

private:
    AudioDevice* aud_dev;
    UdpSocket* socket;
    bool is_stop;
signals:

public slots:

};

#endif // NETPLAYTHREAD_H
