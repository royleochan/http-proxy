#include "Server.h"

#define BUFF_SIZE 8192

const std::string IMAGE_SUB_REQ = "GET http://ocna0.d2.comp.nus.edu.sg:50000/change.jpg HTTP/1.1\r\nHost: ocna0.d2.comp.nus.edu.sg:50000\r\nAccept: */*\r\n\r\n";
const std::string ATTACKER_MODE = "You are being attacked";
const std::string BAD_REQUEST = "Could not parse http request";

Server::Server(int port, bool isImageSubMode, bool isAttackerMode) : connSocket(Socket::createSocket(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, nullptr, SERVER_SOCKET)),
                                                                     port(port), isImageSubMode(isImageSubMode), isAttackerMode(isAttackerMode)
{
}

std::string Server::createPlainTextResponse(HttpVersion version, HttpStatusCode code, int length, std::string content)
{
    std::string v = HttpUtil::httpVersionToString(version);
    std::string statusCode = HttpUtil::httpStatusCodeToString(code);
    return v + " " + std::to_string(static_cast<int>(code)) + statusCode + "\nContent-Type: text/plain\nContent-Length: " + std::to_string(length) + "\n\n" + content;
}

std::pair<std::string, int> Server::getImageSub()
{
    HttpRequest req = HttpRequest::parseStringToHttpRequest(IMAGE_SUB_REQ);
    hostent *h = gethostbyname(req.getUrl().getHost().c_str());
    Socket clientSock = Socket::createSocket(AF_INET, SOCK_STREAM, 0, req.getUrl().getPort(), INADDR_ANY, h, CLIENT_SOCKET);
    write(clientSock.getSocketFd(), IMAGE_SUB_REQ.data(), IMAGE_SUB_REQ.length());

    return receive(clientSock.getSocketFd(), true);
}

std::pair<std::string, int> Server::receive(int socket, bool isSubbed)
{
    // read until at least entire response header is received
    size_t r = 0;
    char *endHeader = NULL;
    char buffer[BUFF_SIZE] = {0};
    do
    {
        r += recv(socket, buffer, BUFF_SIZE, 0);
        if (r <= 0)
            break;
        endHeader = strstr(buffer, "\r\n\r\n");
    } while (!endHeader);

    // parse response to get headers + content
    HttpResponse response = HttpResponse::parseStringToHttpResponse(buffer);

    // allocate buffer of sufficient size to read entire content
    size_t currContentLength = r - response.getHeaderSize();
    int responseContentLength = response.getContentLength();
    char *newBuffer = new char[BUFF_SIZE + responseContentLength];
    std::memcpy(newBuffer, buffer, r);
    char *ptr = newBuffer + r;

    while (currContentLength != responseContentLength)
    {
        size_t temp = recv(socket, ptr, BUFF_SIZE + responseContentLength, 0);
        if (temp < 0)
            perror("Failed to receive from socket");
        else
        {
            currContentLength += temp;
            ptr += temp;
        }
    }

    std::string result = std::string(newBuffer, currContentLength + response.getHeaderSize()); // https://stackoverflow.com/questions/164168/how-do-you-construct-a-stdstring-with-an-embedded-null
    delete[] newBuffer;

    // sub image if response is of type image
    if (isImageSubMode && response.isContentTypeImage() && !isSubbed)
    {
        return getImageSub();
    }

    return std::pair<std::string, int>(result, currContentLength);
}

std::pair<std::string, int> Server::handleParsedRequest(HttpRequest request, char reqString[], int length)
{
    // if attacker mode, return attack response
    if (isAttackerMode)
    {
        return std::pair<std::string, int>(createPlainTextResponse(request.getVersion(), HttpStatusCode::Ok, ATTACKER_MODE.length(), ATTACKER_MODE), ATTACKER_MODE.length());
    }

    // send request to target server
    hostent *h = gethostbyname(request.getUrl().getHost().c_str());
    Url url = request.getUrl();
    Socket clientSock = Socket::createSocket(AF_INET, SOCK_STREAM, 0, url.getPort(), INADDR_ANY, h, CLIENT_SOCKET);
    std::string minimalReqString = HttpRequest::createMinimalGetReq(url.getReqUrl(), url.getHost() + ":" + std::to_string(url.getPort()), request.getVersion());
    write(clientSock.getSocketFd(), minimalReqString.data(), minimalReqString.length());

    // don't sub favicon
    bool isSubbed = url.getReqUrl().find("favicon.ico") != std::string::npos;

    // helper receive function
    return receive(clientSock.getSocketFd(), isSubbed);
}

void Server::handleClient(int socket)
{
    int r = 0;
    do
    {
        // read until at least entire http request header is received
        r = 0;
        char *endHeader = NULL;
        char buffer[BUFF_SIZE] = {0};
        do
        {
            r += read(socket, buffer, BUFF_SIZE);
            if (r <= 0)
                break;
            endHeader = strstr(buffer, "\r\n\r\n");
        } while (!endHeader);

        // try to parse http request, if able to parse then handle it and write result to socket, else return 400 error
        if (r > 0)
        {
            std::string result;
            try
            {
                HttpRequest req = HttpRequest::parseStringToHttpRequest(buffer);
                std::pair<std::string, int> result = handleParsedRequest(req, buffer, r);

                // handle telemetry
                write(socket, result.first.data(), result.first.size());
                std::string telemetryKey = req.getBaseUrl();
                if (telemetryState.find(telemetryKey) == telemetryState.end())
                {
                    TelemetryHandler *tHandler = new TelemetryHandler(telemetryKey, result.second);
                    telemetryState[telemetryKey] = tHandler;
                    std::thread t = std::thread(&TelemetryHandler::run, tHandler, std::ref(telemetryState));
                    t.detach();
                }
                else
                {
                    TelemetryHandler *h = telemetryState[telemetryKey];
                    TelemetryHandler *newTHandler = new TelemetryHandler(telemetryKey, result.second + h->getSize());
                    h->setTerminate();
                    telemetryState[telemetryKey] = newTHandler;
                    std::thread t = std::thread(&TelemetryHandler::run, newTHandler, std::ref(telemetryState));
                    t.detach();
                }
            }
            catch (...)
            {
                std::string result = createPlainTextResponse(HttpVersion::HTTP_1_1, HttpStatusCode::BadRequest, BAD_REQUEST.length(), BAD_REQUEST);
                write(socket, result.data(), result.size());
            }
        }
    } while (r > 0);

    // close socket if no more bytes to read
    close(socket);
}

void Server::startListening()
{
    if (listen(connSocket.getSocketFd(), 5) < 0)
    {
        perror("Error listening to socket...");
    }
    else
    {
        printf("Proxy listening on port %d\n", port);

        while (true)
        {
            int newSocket, clen;
            struct sockaddr_in caddr;
            clen = sizeof(caddr);
            if ((newSocket = accept(connSocket.getSocketFd(), (struct sockaddr *)&caddr, (socklen_t *)&clen)) < 0)
            {
                perror("Error accepting connection...");
                exit(EXIT_FAILURE);
            }

            // spawn new thread for each connection
            std::thread t = std::thread(&Server::handleClient, this, newSocket);
            t.detach();
        }
    }
}
