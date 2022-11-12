#include "HttpUtil.h"

HttpMethod HttpUtil::getHttpMethod(std::string method)
{
    if (method == "POST")
    {
        return HttpMethod::POST;
    }
    else if (method == "GET")
    {
        return HttpMethod::GET;
    }
    else if (method == "PUT")
    {
        return HttpMethod::PUT;
    }
    else if (method == "PATCH")
    {
        return HttpMethod::PATCH;
    }
    else if (method == "DELETE")
    {
        return HttpMethod::DELETE;
    }
    else if (method == "HEAD")
    {
        return HttpMethod::HEAD;
    }
    else if (method == "CONNECT")
    {
        return HttpMethod::CONNECT;
    }
    else if (method == "OPTIONS")
    {
        return HttpMethod::OPTIONS;
    }
    else if (method == "TRACE")
    {
        return HttpMethod::TRACE;
    }
    else
    {
        throw std::invalid_argument("Invalid HTTP method");
    }
}

HttpVersion HttpUtil::getHttpVersion(std::string version)
{
    if (version == "HTTP/1.0")
    {
        return HttpVersion::HTTP_1_0;
    }
    else if (version == "HTTP/1.1")
    {
        return HttpVersion::HTTP_1_1;
    }
    else
    {
        throw std::invalid_argument("Unsupported HTTP version");
    }
}

std::string HttpUtil::httpVersionToString(HttpVersion version)
{
    switch (version)
    {
    case HttpVersion::HTTP_1_0:
        return "HTTP/1.0";
    case HttpVersion::HTTP_1_1:
        return "HTTP/1.1";
    default:
        return "";
    };
}

std::string HttpUtil::httpStatusCodeToString(HttpStatusCode code)
{
    switch (code)
    {
    case HttpStatusCode::Ok:
        return "OK";
    case HttpStatusCode::Accepted:
        return "Accepted";
    case HttpStatusCode::BadRequest:
        return "Bad Request";
    case HttpStatusCode::Forbidden:
        return "Forbidden";
    case HttpStatusCode::NotFound:
        return "Not Found";
    case HttpStatusCode::MethodNotAllowed:
        return "Method Not Allowed";
    case HttpStatusCode::InternalServerError:
        return "Internal Server Error";
    case HttpStatusCode::BadGateway:
        return "Bad Gateway";
    default:
        return "";
    }
}

std::vector<std::string> HttpUtil::splitStringByDelim(const std::string &input, char delim)
{
    std::string temp;
    std::vector<std::string> tokens;
    std::stringstream ss(input);

    while (getline(ss, temp, delim))
    {
        tokens.push_back(temp);
    }

    return tokens;
}

std::unordered_map<std::string, std::string> HttpUtil::parseHeaders(const std::vector<std::string> &headers)
{
    std::unordered_map<std::string, std::string> result;

    for (const auto &header : headers)
    {
        auto index = header.find(':', 0);
        if (index != std::string::npos)
        {
            result[header.substr(0, index)] = header.substr(index + 2);
        }
    }

    return result;
}
