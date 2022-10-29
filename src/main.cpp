#include <iostream>

#include "Server/Server.h"

int main(int argc, char *argv[]) {
    if (argc <= 0) {
        perror("Port must be supplied...");
        exit(EXIT_FAILURE);
    }
    int port = std::stoi(argv[1]);
    Server server = Server(port);
    server.startListening();
}
