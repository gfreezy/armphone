#ifndef NETRECTHREAD_H
#define NETRECTHREAD_H

#include <QThread>

class UdpSocket;
class AudioDevice;
class NetRecThread : public QThread
{
    Q_OBJECT
public:
    explicit NetRecThread(AudioDevice* dev, UdpSocket* socket, QObject *parent = 0);
    void stop_record();

protected:
    void run();

private:
    UdpSocket* socket;
    AudioDevice* aud_dev;
    bool is_stop;

signals:

public slots:

};

#endif // NETRECTHREAD_H
