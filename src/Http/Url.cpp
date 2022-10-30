#include "Url.h"

Url::Url(std::string reqUrl, std::string userAgent, std::string connectionStatus) :
    reqUrl(reqUrl), userAgent(userAgent), connectionStatus(connectionStatus) {
}

std::string Url::getReqUrl() {
    return reqUrl;
}

std::string Url::getUserAgent() {
    return userAgent;
}

std::string Url::getConnectionStatus() {
    return connectionStatus;
}
