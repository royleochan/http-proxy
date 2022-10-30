#pragma once

#include <map>
#include <string>

// Http methods
enum class HttpMethod {
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
enum class HttpVersion {
    HTTP_1_0 = 10,
    HTTP_1_1 = 11
};

// Http status codes
enum class HttpStatusCode {
    Ok = 200,
    Created = 201,
    Accepted = 202,
    NonAuthoritativeInformation = 203,
    NoContent = 204,
    ResetContent = 205,
    PartialContent = 206,
    MultipleChoices = 300,
    MovedPermanently = 301,
    Found = 302,
    NotModified = 304,
    BadRequest = 400,
    Unauthorized = 401,
    Forbidden = 403,
    NotFound = 404,
    MethodNotAllowed = 405,
    RequestTimeout = 408,
    ImATeapot = 418,
    InternalServerError = 500,
    NotImplemented = 501,
    BadGateway = 502,
    ServiceUnvailable = 503,
    GatewayTimeout = 504,
    HttpVersionNotSupported = 505
};


class HttpUtil {
public:
    static HttpMethod getHttpMethod(std::string method);
    static HttpVersion getHttpVersion(std::string version);
};
