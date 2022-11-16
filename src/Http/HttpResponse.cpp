#include "HttpResponse.h"

HttpResponse::HttpResponse(std::unordered_map<std::string, std::string> headers, int headerSize) : headers(headers), headerSize(headerSize)
{
}

HttpResponse HttpResponse::parseStringToHttpResponse(const std::string &responseString)
{
    try
    {
        // separate into 2 parts: response + headers and body
        int dividerIndex = responseString.find("\r\n\r\n");
        std::string resAndHeaders = responseString.substr(0, dividerIndex);
        std::string resBody = responseString.substr(dividerIndex + 4, responseString.length());

        // parse response + headers
        std::string line;
        std::vector<std::string> lines;
        auto stream = std::stringstream(resAndHeaders);

        while (std::getline(stream, line, '\n'))
        {
            line.erase(remove(line.begin(), line.end(), '\r'), line.end());
            lines.push_back(line);
        }

        // parse headers
        std::unordered_map<std::string, std::string> headers = HttpUtil::parseHeaders(std::vector<std::string>(
            lines.begin() + 1, lines.end()));

        return HttpResponse(headers, resAndHeaders.size() + 4);
    }
    catch (...)
    {
        throw std::invalid_argument("Could not create HTTP response");
    }
}

int HttpResponse::getContentLength()
{
    std::string key = "Content-Length";
    if (headers.find(key) == headers.end())
    {
        return -1;
    }
    else
    {
        return std::stoi(headers.at(key));
    }
}

size_t HttpResponse::getHeaderSize()
{
    return headerSize;
}

bool HttpResponse::isContentTypeImage()
{
    std::string key = "Content-Type";
    if (headers.find(key) == headers.end())
    {
        return false;
    }
    else
    {
        std::string contentType = headers.at("Content-Type");
        std::size_t idx = contentType.find("/");
        std::string type = contentType.substr(0, idx);
        return type == "image";
    }
}
