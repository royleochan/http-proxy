#pragma once

#include <unistd.h>
#include <netdb.h>

#include "../Socket/Socket.h"
#include "../Http/HttpUtil.h"
#include "../Http/HttpResponse.h"
#include "../Http/HttpRequest.h"

class Server {
private:
    Socket connSocket;
    int port;
    bool isImageSubMode;
    bool isAttackerMode;
    static std::string createPlainTextResponse(HttpVersion version, HttpStatusCode code, int length, std::string content) ;
    std::string handleRequest(HttpRequest request);
    void logRequest(HttpRequest request, size_t responseSize);
public:
    explicit Server(int port, bool isImageSubMode, bool isAttackerMode);
    void startListening();
};
