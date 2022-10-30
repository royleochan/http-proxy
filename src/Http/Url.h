#pragma once

#include <string>

class Url {
private:
    std::string reqUrl;
    std::string userAgent;
    std::string connectionStatus;
public:
    Url(std::string reqUrl, std::string userAgent, std::string connectionStatus);
    std::string getReqUrl();
    std::string getUserAgent();
    std::string getConnectionStatus();
};
