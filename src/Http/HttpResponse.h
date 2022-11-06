#pragma once

#include "HttpUtil.h"

class HttpResponse {
private:
    size_t headerSize;
    std::unordered_map<std::string, std::string> headers;
    HttpResponse(std::unordered_map<std::string, std::string> headers, int headerSize);
public:
    static HttpResponse parseStringToHttpResponse(const std::string& responseString);
    int getContentLength();
    size_t getHeaderSize();
};
