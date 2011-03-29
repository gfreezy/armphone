#ifndef DATASOCKET_H
#define DATASOCKET_H

#include <QUdpSocket>

class DataSocket : public QUdpSocket
{
    Q_OBJECT
public:
    explicit DataSocket(QObject *parent = 0);
    void create(quint16 port);
    void destroy();
    virtual void sendData()=0;
    void stopSendingData();

protected:
    bool stop_sending;
    bool stopped;
};

#endif // DATASOCKET_H
