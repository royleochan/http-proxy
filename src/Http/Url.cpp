#include "Url.h"

Url::Url(int port, std::string host, std::string reqUrl) : port(port), host(host), reqUrl(reqUrl)
{
}

int Url::getPort()
{
    return port;
}

std::string Url::getHost()
{
    return host;
}

std::string Url::getReqUrl()
{
    return reqUrl;
}
