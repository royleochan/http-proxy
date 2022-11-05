#pragma once

#include <unistd.h>
#include <netdb.h>

#include "../Socket/Socket.h"
#include "../Http/HttpRequest.h"
#include "../Http/HttpUtil.h"

class Server {
private:
    Socket connSocket;
    int port;
    bool isImageSubMode;
    bool isAttackerMode;
    static std::string createPlainTextResponse(HttpVersion version, HttpStatusCode code, int length, std::string content) ;
    std::string handleRequest(HttpRequest request);
public:
    explicit Server(int port, bool isImageSubMode, bool isAttackerMode);
    void startListening();
};
