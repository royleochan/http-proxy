#pragma once

#include <vector>
#include <sstream>
#include <unordered_map>

#include "Url.h"
#include "HttpUtil.h"

class HttpRequest {
private:
    Url url;
    HttpMethod verb;
    HttpVersion version;
    std::string content;
    HttpRequest(HttpMethod verb, Url url, HttpVersion version, std::string content);
    static std::unordered_map<std::string, std::string> parseHeaders(const std::vector<std::string>& headers);
public:
    static HttpRequest parseStringToHttpRequest(const std::string& requestString);
    static std::string createMinimalGetReq(std::string endpoint, std::string host, HttpVersion version);
    HttpVersion getVersion();
    Url getUrl();
};
