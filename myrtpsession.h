#ifndef MYRTPSESSION_H
#define MYRTPSESSION_H

#include <audiodevice.h>
#include <jrtplib3/rtpsession.h>
#include <jrtplib3/rtppacket.h>
#include <jrtplib3/rtpudpv4transmitter.h>
#include <jrtplib3/rtpipv4address.h>
#include <jrtplib3/rtpsessionparams.h>
#include <jrtplib3/rtperrors.h>
#include <jrtplib3/rtpsourcedata.h>

class MyRTPSession: public RTPSession
{
public:
    MyRTPSession(AudioDevice* aud);

protected:
    void OnPollThreadStep();
    void ProcessRTPPacket(const RTPSourceData &srcdat,
                          const RTPPacket &rtppack);

private:
    AudioDevice* aud;
};

#endif // MYRTPSESSION_H
