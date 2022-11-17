#pragma once

#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <thread>
#include <signal.h>
#include <csignal>
#include <unordered_set>

#include "../Socket/Socket.h"
#include "../Http/HttpUtil.h"
#include "../Http/HttpResponse.h"
#include "../Http/HttpRequest.h"
#include "TelemetryHandler.h"

class Server
{
private:
    int port;
    Socket connSocket;
    bool isImageSubMode;
    bool isAttackerMode;
    std::unordered_map<std::string, TelemetryHandler *> telemetryState;
    static std::string createPlainTextResponse(HttpVersion version, HttpStatusCode code, int length, std::string content);
    std::pair<std::string, int> getImageSub();
    std::pair<std::string, int> receive(int socket, bool isSubbed);
    std::pair<std::string, int> handleParsedRequest(HttpRequest request, char reqString[], int length);
    void handleClient(int socket);

public:
    explicit Server(int port, bool isImageSubMode, bool isAttackerMode);
    void startListening();
};
