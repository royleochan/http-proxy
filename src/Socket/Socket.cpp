#include "Socket.h"

const std::string CLIENT_SOCKET = "CLIENT";
const std::string SERVER_SOCKET = "SERVER";

Socket::Socket(int communicationDomain, int serviceType, int protocol, int port, u_long interface, hostent *h, std::string socketType)
{
    // Define address
    memset((char *)&address, 0, sizeof(address));
    address.sin_family = communicationDomain;
    address.sin_port = htons(port);
    if (socketType == SERVER_SOCKET)
    {
        address.sin_addr.s_addr = htonl(interface);
    }
    else
    {
        memcpy((char *)&address.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    }

    // Create socket
    if ((sockFd = socket(communicationDomain, serviceType, protocol)) < 0)
    {
        perror("Error creating socket...");
    }
}

Socket::~Socket()
{
    close(sockFd);
}

Socket Socket::createSocket(int communicationDomain, int serviceType, int protocol, int port, u_long interface,
                            hostent *h, std::string socketType)
{
    Socket socket = {communicationDomain, serviceType, protocol, port, interface, h, socketType};
    if (socketType == CLIENT_SOCKET)
    {
        // Connect socket
        if ((connect(socket.getSocketFd(), (struct sockaddr *)socket.getAddress(), sizeof(address))) < 0)
        {
            perror("Error connecting socket...");
        }
    }
    else if (socketType == SERVER_SOCKET)
    {
        // Bind socket
        if ((bind(socket.getSocketFd(), (struct sockaddr *)socket.getAddress(), sizeof(address))) < 0)
        {
            perror("Error binding socket...");
        }
    }
    else
    {
        perror("Socket type not supported...");
        exit(EXIT_FAILURE);
    }
    return socket;
}

int Socket::getSocketFd() const
{
    return sockFd;
}

sockaddr_in *Socket::getAddress()
{
    return &address;
}
