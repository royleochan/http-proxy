#pragma once

#include <string>

class Url {
private:
    int port;
    std::string host;
public:
    Url(int port, std::string host);
    int getPort();
    std::string getHost();
};
