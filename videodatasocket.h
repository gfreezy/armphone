#ifndef VIDEODATASOCKET_H
#define VIDEODATASOCKET_H

#include "datasocket.h"
#include "videodevice.h"

class VideoDataSocket : public DataSocket
{
    Q_OBJECT
public:
    explicit VideoDataSocket(const QString& dev, QObject* parent=0);
    virtual ~VideoDataSocket();
    virtual void sendData();
    void setDisplayWidget();

private:
    void encode(char* buf1, quint64 len1, char* buf2, quint64* len2);
    void decode(char* buf1, quint64 len1, char* buf2, quint64* len2);
    void yuyvToRGB(char* buf1, quint64 len1, char* buf2, quint64* len2);
    void updateDisplayWidget(char* buf, quint64 len);

    VideoDevice* viddev;
    const QString dev;

public slots:
    void ready_read();
};

#endif // VIDEODATASOCKET_H
