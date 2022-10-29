#pragma once

#include <unistd.h>

#include "../Socket/Socket.h"

class Server {
private:
    Socket connSocket;
    int port;
public:
    explicit Server(int port);
    void startListening();
};
