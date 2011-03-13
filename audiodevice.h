#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H
#include <unistd.h>

class AudioDevice
{
public:
    AudioDevice(char* dev = "/dev/dsp");
    ~AudioDevice(void);
    int open_device(void);
    int init(void);
    void close_device(void);
    void set_sample_rate(int);
    void set_sample_size(int);
    void set_channels(int);
    void set_fragments(int, int);
    int read_data(short*, size_t);
    int write_data(const short*, size_t);

private:
    char* dev;
    int fd, sample_rate, sample_size, channels, fragsize, fragnum;
};

#endif // AUDIODEVICE_H
