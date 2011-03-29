#include "myrtpsession.h"

MyRTPSession::MyRTPSession(AudioDevice *aud)
{
    this->aud = aud;
}

void MyRTPSession::OnPollThreadStep()
{
    BeginDataAccess();

    if(GotoFirstSourceWithData())
    {
        do
        {
            RTPPacket *pack;
            RTPSourceData* srcdat;

            srcdat = GetCurrentSourceInfo();

            while((pack = GetNextPacket()) != NULL)
            {
                ProcessRTPPacket(*srcdat, *pack);
                DeletePacket(pack);
            }
        }while(GotoNextSourceWithData());
    }

    EndDataAccess();
}

void MyRTPSession::ProcessRTPPacket(const RTPSourceData &srcdat, const RTPPacket &rtppack)
{
    uint8_t* buf;
    size_t len;

    buf = rtppack.GetPayloadData();
    len = rtppack.GetPayloadLength();

    if(-1 == aud->write_data((short*)buf, len))
    {
        perror("write");
        exit(EXIT_FAILURE);
    }
}
