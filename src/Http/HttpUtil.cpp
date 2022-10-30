#include "HttpUtil.h"

HttpMethod HttpUtil::getHttpMethod(std::string method) {
    if (method == "POST") {
        return HttpMethod::POST;
    } else if (method == "GET") {
        return HttpMethod::GET;
    } else if (method == "PUT") {
        return HttpMethod::PUT;
    } else if (method == "PATCH") {
        return HttpMethod::PATCH;
    } else if (method == "DELETE") {
        return HttpMethod::DELETE;
    } else if (method == "HEAD") {
        return HttpMethod::HEAD;
    } else if (method == "CONNECT") {
        return HttpMethod::CONNECT;
    } else if (method == "OPTIONS") {
        return HttpMethod::OPTIONS;
    } else if (method == "TRACE") {
        return HttpMethod::TRACE;
    } else {
        throw std::invalid_argument("Invalid HTTP method");
    }
}

HttpVersion HttpUtil::getHttpVersion(std::string version) {
    if (version == "HTTP/1.0") {
        return HttpVersion::HTTP_1_0;
    } else if (version == "HTTP/1.1") {
        return HttpVersion::HTTP_1_1;
    } else {
        throw std::invalid_argument("Unsupported HTTP version");
    }
}
