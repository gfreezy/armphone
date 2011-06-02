#ifndef VIDEODATASOCKET_H
#define VIDEODATASOCKET_H

#include "datasocket.h"
#include "videodevice.h"
#include "videowidget.h"
#include "huffman.h"

struct FrameData // total size 20 * 1024
{
    quint8 header[175];
    quint8 dht[DHT_SIZE];
    quint8 pic_data[20 * 1024 - DHT_SIZE - 175 - 16];
    //16 is the space that 'timestamp' and 'len' occupies
    quint64 timestamp;
    quint64 len; // the size of the picture, not the struct
};

class VideoDataSocket : public DataSocket
{
    Q_OBJECT
public:
    explicit VideoDataSocket(const QString& dev, QObject* parent=0);
    virtual ~VideoDataSocket();
    virtual void sendData();
    void setDisplayWidget(VideoWidget* widget);

private:
    void encode(char* buf1, quint64 len1, char* buf2, quint64* len2);
    void decode(char* buf1, quint64 len1, char* buf2, quint64* len2);
    void yuyvToRGB(char* buf1, quint64 len1, char* buf2, quint64* len2);
    void updateDisplayWidget(FrameData* buf);
    void addHuffmanHeader(char* buf, quint64 len, char* buf2, quint64* );

    VideoDevice* viddev;
    const QString dev;
    VideoWidget *vw;
    FrameData *recv_buf;
    FrameData *send_buf;

public slots:
    void ready_read();
    void display_error(QString err);
};

#endif // VIDEODATASOCKET_H
