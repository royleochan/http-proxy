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

void Server::logRequest(HttpRequest request, size_t responseSize)
{
    printf("%s, %zu\n", request.getUrl().getReqUrl().c_str(), responseSize);
}

std::string Server::getImageSub()
{
    HttpRequest req = HttpRequest::parseStringToHttpRequest(IMAGE_SUB_REQ);
    hostent *h = gethostbyname(req.getUrl().getHost().c_str());
    Socket clientSock = Socket::createSocket(AF_INET, SOCK_STREAM, 0, req.getUrl().getPort(), INADDR_ANY, h, CLIENT_SOCKET);
    write(clientSock.getSocketFd(), IMAGE_SUB_REQ.data(), IMAGE_SUB_REQ.length());

    return receive(clientSock.getSocketFd(), true);
}

std::string Server::receive(int socket, bool isSubbed)
{
    char buffer[BUFF_SIZE] = {0};
    size_t r = recv(socket, buffer, BUFF_SIZE, 0);

    if (r < 0)
    {
        perror("Failed to receive from socket");
    }

    HttpResponse response = HttpResponse::parseStringToHttpResponse(buffer);

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

    if (isImageSubMode && response.isContentTypeImage() && !isSubbed)
    {
        return getImageSub();
    }

    return result;
}

std::string Server::handleParsedRequest(HttpRequest request, char reqString[], int length)
{
    if (isAttackerMode)
    {
        return createPlainTextResponse(request.getVersion(), HttpStatusCode::Ok, ATTACKER_MODE.length(), ATTACKER_MODE);
    }

    hostent *h = gethostbyname(request.getUrl().getHost().c_str());
    Url url = request.getUrl();
    Socket clientSock = Socket::createSocket(AF_INET, SOCK_STREAM, 0, url.getPort(), INADDR_ANY, h, CLIENT_SOCKET);
    std::string test = HttpRequest::createMinimalGetReq(url.getReqUrl(), url.getHost() + ":" + std::to_string(url.getPort()), request.getVersion());
    write(clientSock.getSocketFd(), test.data(), test.length());

    bool isSubbed = url.getReqUrl().find("favicon.ico") != std::string::npos; // don't sub favicon

    std::string result = receive(clientSock.getSocketFd(), isSubbed);

    return result;
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

        // try to parse http request, if able to parse then handle it, else return 400 error
        if (r > 0)
        {
            std::string result;
            try
            {
                HttpRequest req = HttpRequest::parseStringToHttpRequest(buffer);
                std::cout << std::hash<std::thread::id>{}(std::this_thread::get_id()) << std::endl;
                std::cout << req.getUrl().getReqUrl() << std::endl;
                result = handleParsedRequest(req, buffer, r);
            }
            catch (...)
            {
                result = createPlainTextResponse(HttpVersion::HTTP_1_1, HttpStatusCode::BadRequest, BAD_REQUEST.length(), BAD_REQUEST);
            }

            write(socket, result.data(), result.size());
        }
    } while (r > 0);
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
        printf("Proxy listening on port %d with socket %d\n", port, connSocket.getSocketFd());

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

            printf("New connection on socket %d\n", newSocket);
            std::thread t = std::thread(&Server::handleClient, this, newSocket);
            t.detach();
        }
    }
}
