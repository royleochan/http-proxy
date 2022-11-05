#include "Server.h"

#define BUFF_SIZE 4096

const std::string ATTACKER_MODE = "You are being attacked";
const std::string BAD_REQUEST = "Could not parse http request";

Server::Server(int port, bool isImageSubMode, bool isAttackerMode) : connSocket(Socket::createSocket(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, nullptr, SERVER_SOCKET)),
    port(port), isImageSubMode(isImageSubMode), isAttackerMode(isAttackerMode) {
}

std::string Server::createPlainTextResponse(HttpVersion version, HttpStatusCode code, int length, std::string content) {
    std::string v = HttpUtil::httpVersionToString(version);
    std::string statusCode = HttpUtil::httpStatusCodeToString(code);
    return v + " " + std::to_string(static_cast<int>(code)) + statusCode + "\nContent-Type: text/plain\nContent-Length: " + std::to_string(length) + "\n\n" + content;
}

std::string Server::handleRequest(HttpRequest request, std::string reqString) const {
    if (isAttackerMode) {
       return createPlainTextResponse(request.getVersion(), HttpStatusCode::Ok, ATTACKER_MODE.length(), ATTACKER_MODE);
    }

    hostent* h = gethostbyname(request.getUrl().getHost().c_str());
    Socket clientSock = Socket::createSocket(AF_INET, SOCK_STREAM, 0, request.getUrl().getPort(), INADDR_ANY, h, CLIENT_SOCKET);
    reqString = "GET http://ocna0.d2.comp.nus.edu.sg:50000/tc2/ HTTP/1.1\r\nHost: ocna0.d2.comp.nus.edu.sg:50000\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:106.0) Gecko/20100101 Firefox/106.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\r\nAccept-Language: en-GB,en;q=0.5\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\n\r\n";
    write(clientSock.getSocketFd(), reqString.data(),reqString.length());
    char buffer[BUFF_SIZE] = {0};
    size_t total = 0;
    while (true) {
        ssize_t sz = recv(clientSock.getSocketFd(), buffer, BUFF_SIZE, 0);
        if (sz == -1) perror("receive failed");
        if (sz == 0) break;
        total += sz;
    }

    return {buffer};
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

        char buffer[BUFF_SIZE] = {0};
        int numBytesRead = read(newSocket, buffer, BUFF_SIZE);
        std::string result;

        try {
            HttpRequest req = HttpRequest::parseStringToHttpRequest(buffer);
            result = handleRequest(req, std::string(buffer).substr(0, numBytesRead));
        } catch (...) {
            result = createPlainTextResponse(HttpVersion::HTTP_1_1, HttpStatusCode::BadRequest, BAD_REQUEST.length(), BAD_REQUEST);
        }

        write(newSocket, result.data(), result.size());
        close(newSocket);
    }
}
