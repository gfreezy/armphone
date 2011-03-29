#ifndef AUDIODATASOCKET_H
#define AUDIODATASOCKET_H

#include "datasocket.h"
#include "audiodevice.h"

class AudioDataSocket : public DataSocket
{
    Q_OBJECT
public:
    explicit AudioDataSocket(const QString& aud, QObject* parent=0);
    virtual ~AudioDataSocket();
    virtual void sendData();

private:
    const QString aud;
    AudioDevice* auddev;

public slots:
    void ready_read();
};

#endif // AUDIODATASOCKET_H
