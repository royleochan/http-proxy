#pragma once

#include "HttpUtil.h"

class HttpResponse {
private:
    std::unordered_map<std::string, std::string> headers;
    HttpResponse(std::unordered_map<std::string, std::string> headers);
public:
    static HttpResponse parseStringToHttpResponse(const std::string& responseString);
    int getContentLength();
};
