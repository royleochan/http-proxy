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
    HttpRequest(HttpMethod verb, Url url);
    static std::unordered_map<std::string, std::string> parseHeaders(std::vector<std::string> headers);
public:
    static HttpRequest parseStringToHttpRequest(std::string requestString);
};
