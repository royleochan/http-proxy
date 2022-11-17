#pragma once

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

// Http methods
enum class HttpMethod
{
    POST,
    GET,
    PUT,
    PATCH,
    DELETE,
    HEAD,
    CONNECT,
    OPTIONS,
    TRACE,
};

// Only support HTTP/1.0 and HTTP/1.1
enum class HttpVersion
{
    HTTP_1_0 = 10,
    HTTP_1_1 = 11
};

// Http status codes
enum class HttpStatusCode
{
    Ok = 200,
    Created = 201,
    Accepted = 202,
    NotModified = 304,
    BadRequest = 400,
    Unauthorized = 401,
    Forbidden = 403,
    NotFound = 404,
    MethodNotAllowed = 405,
    RequestTimeout = 408,
    InternalServerError = 500,
    BadGateway = 502,
};

class HttpUtil
{
public:
    static HttpMethod getHttpMethod(std::string method);
    static HttpVersion getHttpVersion(std::string version);
    static HttpStatusCode getHttpStatusCode(std::string version);
    static std::string httpVersionToString(HttpVersion version);
    static std::string httpStatusCodeToString(HttpStatusCode code);
    static std::vector<std::string> splitStringByDelim(const std::string &input, char delim);
    static std::unordered_map<std::string, std::string> parseHeaders(const std::vector<std::string> &headers);
};
