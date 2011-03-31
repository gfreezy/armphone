#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H
#include <unistd.h>
#include <QString>
#include <QObject>

class AudioDevice : public QObject
{
    Q_OBJECT
public:
    AudioDevice(const QString& dev);
    ~AudioDevice(void);
    int open_device(void);
    int init_device(void);
    void close_device(void);
    void set_sample_rate(int);
    void set_sample_size(int);
    void set_channels(int);
    void set_fragments(int, int);
    int read_data(short*, size_t);
    int write_data(const short*, size_t);

private:
    const QString dev;
    int fd, sample_rate, sample_size, channels, fragsize, fragnum;

signals:
    void display_error(QString err);
};

#endif // AUDIODEVICE_H
