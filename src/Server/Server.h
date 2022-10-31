#pragma once

#include <unistd.h>

#include "../Socket/Socket.h"
#include "../Http/HttpRequest.h"

class Server {
private:
    Socket connSocket;
    int port;
    bool isImageSubMode;
    bool isAttackerMode;
    char* handleRequest(HttpRequest request) const;
public:
    explicit Server(int port, bool isImageSubMode, bool isAttackerMode);
    void startListening();
};
