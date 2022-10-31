#include "Server.h"

Server::Server(int port, bool isImageSubMode, bool isAttackerMode) : connSocket(Socket::createSocket(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, SERVER_SOCKET)),
    port(port), isImageSubMode(isImageSubMode), isAttackerMode(isAttackerMode) {
}

char* Server::handleRequest(HttpRequest request) const {
    if (isAttackerMode) {
        return "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 22\n\nYou are being attacked";
    }
    return "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 21\n\nNeed to proxy request";
}

void Server::startListening() {
    if (listen(connSocket.getSocketFd(), 5) < 0) {
        perror("Error listening to socket...");
    } else {
        printf("Proxy listening on port %d\n", port);
    }

    while (true) {
        int newSocket;
        if ((newSocket = accept(connSocket.getSocketFd(), (struct sockaddr *)connSocket.getAddress(), (socklen_t*)connSocket.getAddress())) < 0)
        {
            perror("Error accepting connection...");
            exit(EXIT_FAILURE);
        }

        char buffer[1024] = {0};
        read(newSocket, buffer, 1024);
        printf("%s", buffer);
        HttpRequest req = HttpRequest::parseStringToHttpRequest(buffer);

        const char* result = handleRequest(req);
        write(newSocket , result , strlen(result));
        close(newSocket);
    }
}
