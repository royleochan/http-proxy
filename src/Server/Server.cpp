#include "Server.h"

#define MAX_REQ_THREADS 10
#define BUFF_SIZE 8192

const std::string ATTACKER_MODE = "You are being attacked";
const std::string BAD_REQUEST = "Could not parse http request";

Server::Server(int port, bool isImageSubMode, bool isAttackerMode) : connSocket(Socket::createSocket(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, nullptr, SERVER_SOCKET)),
                                                                     port(port), isImageSubMode(isImageSubMode), isAttackerMode(isAttackerMode)
{
}

void Server::logRequest(HttpRequest request, size_t responseSize)
{
    printf("%s, %zu\n", request.getUrl().getReqUrl().c_str(), responseSize);
}

void Server::handleRequest(int socket)
{
    char buffer[BUFF_SIZE] = {0};
    read(socket, buffer, BUFF_SIZE);
    std::string result;

    try
    {
        HttpRequest req = HttpRequest::parseStringToHttpRequest(buffer);
        result = handleParsedRequest(req);
    }
    catch (...)
    {
        result = createPlainTextResponse(HttpVersion::HTTP_1_1, HttpStatusCode::BadRequest, BAD_REQUEST.length(), BAD_REQUEST);
    }

    write(socket, result.data(), result.size());
}

std::string Server::createPlainTextResponse(HttpVersion version, HttpStatusCode code, int length, std::string content)
{
    std::string v = HttpUtil::httpVersionToString(version);
    std::string statusCode = HttpUtil::httpStatusCodeToString(code);
    return v + " " + std::to_string(static_cast<int>(code)) + statusCode + "\nContent-Type: text/plain\nContent-Length: " + std::to_string(length) + "\n\n" + content;
}

std::string Server::handleParsedRequest(HttpRequest request)
{
    if (isAttackerMode)
    {
        return createPlainTextResponse(request.getVersion(), HttpStatusCode::Ok, ATTACKER_MODE.length(), ATTACKER_MODE);
    }

    hostent *h = gethostbyname(request.getUrl().getHost().c_str());
    Url url = request.getUrl();
    Socket clientSock = Socket::createSocket(AF_INET, SOCK_STREAM, 0, url.getPort(), INADDR_ANY, h, CLIENT_SOCKET);
    std::string reqString = HttpRequest::createMinimalGetReq(url.getReqUrl(), url.getHost() + ":" + std::to_string(url.getPort()), request.getVersion());
    write(clientSock.getSocketFd(), reqString.data(), reqString.length());

    char buffer[BUFF_SIZE] = {0};
    size_t r = recv(clientSock.getSocketFd(), buffer, BUFF_SIZE, 0);
    HttpResponse response = HttpResponse::parseStringToHttpResponse(buffer);
    size_t currContentLength = r - response.getHeaderSize();
    int responseContentLength = response.getContentLength();
    char *newBuffer = new char[BUFF_SIZE + responseContentLength];
    std::memcpy(newBuffer, buffer, r);
    char *ptr = newBuffer + r;

    while (currContentLength != responseContentLength)
    {
        size_t temp = recv(clientSock.getSocketFd(), ptr, BUFF_SIZE + responseContentLength, 0);
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
    logRequest(request, currContentLength);
    return result;
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
    }

    while (true)
    {
        // TODO: use select for concurrent connections
        // int opts = 1
        // setsockopt(newSocket, SOL_SOCKET, SO_REUSEADDR, &opts, sizeof(opts));
        // opts = (opts | O_NONBLOCK);
        // fcntl(newSocket, F_SETFL, opts);
        int newSocket, clen;
        struct sockaddr_in caddr;
        clen = sizeof(caddr);
        if ((newSocket = accept(connSocket.getSocketFd(), (struct sockaddr *)&caddr, (socklen_t *)&clen)) < 0)
        {
            perror("Error accepting connection...");
            exit(EXIT_FAILURE);
        }

        printf("New connection on socket %d\n", newSocket);

        // handle concurrent requests with threading
        for (int i = 0; i < MAX_REQ_THREADS; i++)
        {
            // TODO: use thread pool instead of hard coded number of threads
            std::thread t = std::thread(&Server::handleRequest, this, newSocket);
            t.detach();
        }

        // TODO: close(newSocket) => after a set timeout or client closes socket
    }
}
