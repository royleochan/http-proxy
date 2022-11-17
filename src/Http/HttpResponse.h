#pragma once

#include "HttpUtil.h"
#include <algorithm>

class HttpResponse
{
private:
    size_t headerSize;
    HttpStatusCode status;
    std::unordered_map<std::string, std::string> headers;
    HttpResponse(std::unordered_map<std::string, std::string> headers, int headerSize, HttpStatusCode status);

public:
    static HttpResponse parseStringToHttpResponse(const std::string &responseString);
    int getContentLength();
    size_t getHeaderSize();
    bool isContentTypeImage();
    bool isStatusOk();
};
