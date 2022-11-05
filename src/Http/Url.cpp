#include "Url.h"

Url::Url(int port, std::string host) : port(port), host(host) {
}

int Url::getPort() {
    return port;
}

std::string Url::getHost() {
    return host;
}
