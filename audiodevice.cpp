#include "audiodevice.h"
#include <sys/soundcard.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define FRAG(x, y) ((((x) & 0xFFFF) << 16) | ((y) & 0xFFFF))            //x fragnum, y fragsize

AudioDevice::AudioDevice(char* dev)
{
    this->dev = dev;
    this->channels = 2;
    this->sample_rate = 8000;
    this->sample_size = AFMT_S16_LE;
    this->fragnum = 40;
    this->fragsize = 32 * sizeof(short) * channels;
}

AudioDevice::~AudioDevice(void)
{
}

void AudioDevice::set_channels(int channels)
{
    this->channels = channels;
}

void AudioDevice::set_sample_rate(int rate)
{
    this->sample_rate = rate;
}

void AudioDevice::set_sample_size(int size)
{
    this->sample_size = size;
}

void AudioDevice::set_fragments(int size, int num)
{
    this->fragnum = num;
    this->fragsize = size;
}

int AudioDevice::init_device(void)
{
    int buffarg;
    int frag = 0;
    fragnum &= 0xFFFF;
    for(buffarg = 0; fragsize > 1; buffarg++, fragsize >>= 1)
        ;
    frag = FRAG(fragnum, buffarg);
    if(-1 == ioctl(fd, SNDCTL_DSP_SETFRAGMENT, &frag))
    {
        perror("setting fragments");
        return -1;
    }
    fprintf(stderr,"set frag\n");

    if(-1 == ioctl(fd, SNDCTL_DSP_SETFMT, &sample_size))
    {
        perror("setting sample size");
        return -1;
    }
    fprintf(stderr,"set sample\n");

    if(-1 == ioctl(fd, SNDCTL_DSP_CHANNELS, &channels))
    {
        perror("setting channels");
        return -1;
    }
    fprintf(stderr, "set channels\n");

    if(-1 == ioctl(fd, SNDCTL_DSP_SPEED, &sample_rate))
    {
        perror("setting sample rate");
        return -1;
    }
    fprintf(stderr, "set rate\n");



    audio_buf_info bi;
    if(-1 == ioctl(fd, SNDCTL_DSP_GETOSPACE, &bi))
    {
        perror("getospace");
        return -1;
    }
    fprintf(stderr,"bytes %d,fragments %d,fragsize %d, fragstotal %d\n",
            bi.bytes, bi.fragments, bi.fragsize, bi.fragstotal);
    return 0;

}

int AudioDevice::open_device(void)
{
    if(-1 == (fd = open(dev, O_RDWR, 0)))
    {
        perror("open /dev/dsp");
        return -1;
    }
    fprintf(stderr,"open device success\n");
    return fd;
}

void AudioDevice::close_device(void)
{
    if(fd != -1)
        close(fd);
}

int AudioDevice::read_data(short* buf, size_t size)
{
    size_t i;
    short dblbuf[1024];

    if(-1 == read(fd, (void*)dblbuf, size * 2))
    {
        perror("read data");
        return -1;
    }

    //balance between the two channels
    //the first element is from left channel,the follow is from right channel
    for (i = 0; i < size/sizeof(short); i++)
            buf[i] = ((int)dblbuf[2*i] + (int)dblbuf[2*i+1])/2;

    return 0;
}

int AudioDevice::write_data(const short* buf, size_t size)
{
    size_t i;
    int res;
    size_t free_space;
    size_t frag_size;
    size_t used_space;
    short dblbuf[1024];
    audio_buf_info bi;
    size_t s = size * 2;                    //array dbubuf's size is size * 2, because we use 2 channels

    for (i = 0; i < size/sizeof(short); i++)
    {
            dblbuf[2*i] = buf[i];
            dblbuf[2*i+1] = buf[i];
    }

    res = ioctl(fd, SNDCTL_DSP_GETOSPACE, &bi);
    if (res == -1)
    {
            perror("SNDCTL_DSP_GETOSPACE");
            return -1;
    }

    /*Note:
     * The audio_buf_info field contains the following fields:
     * bytes returns the number of bytes that can be written without blocking.
     * fragments (obsolete) returns the number of full fragments that can be written without blocking.
     * fragsize is the current fragment size in playback direction.
     * fragstotal is the number of buffer fragments allocated for playback.
     */
    frag_size = bi.fragsize;
    free_space = bi.bytes;
    if (free_space < s)
            printf("play overrun\n"); // play speed too slow, so plz write slowly

    res = write(fd, dblbuf, s);
//    printf ("write %d bytes\n", res);
    if (res == -1)
    {
        perror("auddev: writing audio device error");
        return -1;
    }

    used_space = frag_size * bi.fragstotal - free_space; /* total size - free size */

    if (used_space < 3*frag_size)
    {
            fprintf(stderr, "play underrun\n"); //play speed too high, so plz write quickly
            res = write(fd, dblbuf, s);
            if(res == -1)
            {
                perror("writing audio device error");
                return -1;
            }
    }

    return 0;

}
