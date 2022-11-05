#include "HttpRequest.h"

HttpRequest::HttpRequest(HttpMethod verb, Url url, HttpVersion version, std::string content) : verb(verb), url(url), version(version), content(content) {
}

std::vector<std::string> splitStringByDelim(const std::string& input, char delim) {
    std::string temp;
    std::vector<std::string> tokens;
    std::stringstream ss(input);

    while(getline(ss, temp, delim)) {
        tokens.push_back(temp);
    }

    return tokens;
}

std::unordered_map<std::string, std::string> HttpRequest::parseHeaders(const std::vector<std::string>& headers) {
    std::unordered_map<std::string, std::string> result;

    for (const auto& header : headers) {
        auto index = header.find(':', 0);
        if (index != std::string::npos) {
            result[header.substr(0, index)] = header.substr(index + 2);
        }
    }

    return result;
}

HttpRequest HttpRequest::parseStringToHttpRequest(const std::string& requestString) {
    try {
        // separate into 2 parts: request + headers and body
        int dividerIndex = requestString.find("\r\n\r\n");
        std::string reqAndHeaders = requestString.substr(0, dividerIndex);
        std::string reqBody = requestString.substr(dividerIndex + 4, requestString.length());

        // parse req + headers
        std::string line;
        std::vector<std::string> lines;
        auto stream = std::stringstream(reqAndHeaders);

        while(std::getline(stream, line, '\n')) {
            line.erase(remove(line.begin(), line.end(), '\r'), line.end());
            lines.push_back(line);
        }

        // parse req
        std::string req = lines.at(0);
        std::vector<std::string> reqInfo = splitStringByDelim(req, ' ');
        std::string httpVerb = reqInfo.at(0);
        std::string endPoint = reqInfo.at(1);
        HttpVersion version = HttpUtil::getHttpVersion(reqInfo.at(2));

        // parse headers
        std::unordered_map<std::string, std::string> headers = parseHeaders(std::vector<std::string>(
                lines.begin() + 1, lines.end()));

        // parse end point
        int port, pos;
        std::string host;
        std::vector<std::string> endPointInfo = splitStringByDelim(endPoint, ':');
        std::string hostSection = endPointInfo.at(1);
        std::vector<std::string> hostInfo = splitStringByDelim(hostSection, '/');
        host = hostInfo.at(2);
        if (endPointInfo.size() >= 2) {
            std::string temp = endPointInfo.at(2);
            pos = temp.find('/');
            port = stoi(temp.substr(0, pos));
        } else {
            port = 80;
        }

        return {HttpUtil::getHttpMethod(httpVerb), Url(port, host, endPoint), version, reqBody};
    } catch (...) {
        throw std::invalid_argument("Could not create HTTP request");
    }
}

HttpVersion HttpRequest::getVersion() {
    return version;
}

Url HttpRequest::getUrl() {
    return url;
}
