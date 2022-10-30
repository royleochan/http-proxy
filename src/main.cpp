#include <iostream>

#include "Server/Server.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        perror("3 arguments must be supplied: port, imageSubFlag, attackerModeFlag");
        exit(EXIT_FAILURE);
    }

    bool isProxyImage;
    bool isAttackerMode;
    int port = std::stoi(argv[1]);
    std::istringstream(argv[2]) >> isProxyImage;
    std::istringstream(argv[3]) >> isAttackerMode;

    Server server = Server(port);
    server.startListening();
}
