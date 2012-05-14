#ifndef VIDEODATASOCKETTHREAD_H
#define VIDEODATASOCKETTHREAD_H

#include <QThread>
#include "videodatasocket.h"

class VideoDataSocketThread : public QThread
{
    Q_OBJECT
public:
    explicit VideoDataSocketThread(const QString dev="/dev/video", QObject *parent = 0);
    void run();
    void stop();
    void connectToHost(QHostAddress& addr, quint16 port);
    void setDisplayWidget(VideoWidget* vw);

private:
    const QString dev;
    const QString aud;
    VideoDataSocket* vidsock;

signals:

public slots:

};

#endif // VIDEODATASOCKETTHREAD_H
