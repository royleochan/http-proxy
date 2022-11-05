#include "Socket.h"

const std::string CLIENT_SOCKET = "CLIENT";
const std::string SERVER_SOCKET = "SERVER";

Socket::Socket(int communicationDomain, int serviceType, int protocol, int port, u_long interface) {
    // Define address
    memset((char *)&address, 0, sizeof(address));
    address.sin_family = communicationDomain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);
    // Create socket
    if ((sockFd = socket(communicationDomain, serviceType, protocol)) < 0) {
        perror("Error creating socket...");
    }
}

Socket::~Socket() {
    close(sockFd);
}

Socket Socket::createSocket(int communicationDomain, int serviceType, int protocol, int port, u_long interface,
                            std::string socketType) {
    Socket socket = {communicationDomain, serviceType, protocol, port, interface};
    if (socketType == CLIENT_SOCKET) {
        // Connect socket
        if ((connect(socket.getSocketFd(), (struct sockaddr*) socket.getAddress(), sizeof(address))) < 0) {
            perror("Error binding socket...");
        }
    } else if (socketType == SERVER_SOCKET) {
        // Bind socket
        if ((bind(socket.getSocketFd(), (struct sockaddr*) socket.getAddress(), sizeof(address))) < 0) {
            perror("Error binding socket...");
        }
    } else {
        perror("Socket type not supported...");
        exit(EXIT_FAILURE);
    }
    return socket;
}

int Socket::getSocketFd() const {
    return sockFd;
}

sockaddr_in * Socket::getAddress() {
    return &address;
}
