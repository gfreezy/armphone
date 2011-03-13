#ifndef UDPSOCKET_H
#define UDPSOCKET_H
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <net/if.h>
#include <string.h>
#include <stdio.h>

class UdpSocket
{
public:
    UdpSocket();
    void bind_(int);
    int connect_(const char*, int);
    ssize_t read_(void*, size_t);
    void write_(const void*, size_t);
    void close_();
    int get_socket_fd();

private:
    int fd;
    struct sockaddr_in local_addr;
    struct sockaddr_in remote_addr;
    size_t remote_addr_len;
    int local_port;


};

#endif // UDPSOCKET_H
