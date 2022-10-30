#pragma once

#include <string>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

extern const std::string CLIENT_SOCKET;
extern const std::string SERVER_SOCKET;

class Socket {
private:
    int sockFd;
    struct sockaddr_in address;
    Socket(int communicationDomain, int serviceType, int protocol, int port, u_long interface);
public:
    ~Socket();
    static Socket createSocket(int communicationDomain, int serviceType, int protocol, int port, u_long interface, std::string socketType);
    int getSocketFd() const;
    sockaddr_in * getAddress();
};
