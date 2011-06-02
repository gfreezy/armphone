#ifndef AUDIODATASOCKETTHREAD_H
#define AUDIODATASOCKETTHREAD_H

#include <QThread>
#include "audiodatasocket.h"

class AudioDataSocketThread : public QThread
{
    Q_OBJECT
public:
    explicit AudioDataSocketThread(const QString& aud="dev/dsp", QObject *parent = 0);
    void run();
    void stop();
    void connectToHost(QHostAddress& addr, quint16 port);

private:
    AudioDataSocket* audsock;
    const QString aud;

};

#endif // AUDIODATASOCKETTHREAD_H
