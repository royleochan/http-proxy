#pragma once

#include <string>

class Url
{
private:
    int port;
    std::string host;
    std::string reqUrl;

public:
    Url(int port, std::string host, std::string reqUrl);
    int getPort();
    std::string getHost();
    std::string getReqUrl();
};
