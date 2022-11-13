#pragma once

#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <thread>

#include "../Socket/Socket.h"
#include "../Http/HttpUtil.h"
#include "../Http/HttpResponse.h"
#include "../Http/HttpRequest.h"

class Server
{
private:
    Socket connSocket;
    int port;
    bool isImageSubMode;
    bool isAttackerMode;
    static std::string createPlainTextResponse(HttpVersion version, HttpStatusCode code, int length, std::string content);
    void logRequest(HttpRequest request, size_t responseSize);
    std::string getImageSub();
    std::string receive(int socket, bool isSubbed);
    std::string handleParsedRequest(HttpRequest request);
    void handleClient(int socket);

public:
    explicit Server(int port, bool isImageSubMode, bool isAttackerMode);
    void startListening();
};
