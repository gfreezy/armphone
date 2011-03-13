#include "udpsocket.h"

UdpSocket::UdpSocket()
{
    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket error");
    }
    remote_addr_len = sizeof(remote_addr);
}

void UdpSocket::bind_(int port)
{
    local_port = port;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(local_port);
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(fd, (struct sockaddr*) &local_addr, sizeof(local_addr));
}

void UdpSocket::close_()
{
    if(close(fd) < 0)
        perror("close socket");
}

int UdpSocket::connect_(const char* ip, int port)
{
    struct hostent *hptr;
    hptr = gethostbyname(ip);
    if(hptr == NULL)
    {
        fprintf(stderr, "%s, not found\n", ip);
        return -1;
    }
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(port);
    remote_addr.sin_addr.s_addr = *(int *)hptr->h_addr_list[0];
    return 0;
}

ssize_t UdpSocket::read_(void * buf, size_t s)
{
    ssize_t size;
    if((size = recvfrom(fd, buf, s, 0, (struct sockaddr*)&remote_addr, &remote_addr_len)) < 0)
    {
        perror("read socket");
        return -1;
    }
    return size;
}

void UdpSocket::write_(const void * buf, size_t s)
{
    if(sendto(fd, buf, s, 0, (struct sockaddr*)&remote_addr, remote_addr_len) != (ssize_t)s)
    {
        perror("write to socket");
    }
    return;
}

int UdpSocket::get_socket_fd()
{
    return fd;
}
