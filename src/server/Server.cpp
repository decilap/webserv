#include "Server.hpp"

Server::Server() : _running(false) {}

Server::~Server() {
    stop();
}

void Server::setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        flags = 0;
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
        std::cerr << "fcntl error: " << strerror(errno) << std::endl;
}

void Server::createListeningSocket(int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "socket() failed: " << strerror(errno) << std::endl;
        return;
    }

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
    addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "bind() failed: " << strerror(errno) << std::endl;
        close(sockfd);
        return;
    }

    if (listen(sockfd, SOMAXCONN) < 0) {
        std::cerr << "listen() failed: " << strerror(errno) << std::endl;
        close(sockfd);
        return;
    }

    setNonBlocking(sockfd);
    _listenSockets.push_back(sockfd);
    std::cout << "[Server] Listening on 127.0.0.1:" << port << std::endl;
}

void Server::start() {
    if (_running)
        return;
    _running = true;

    // Pour le sprint 1 : on écoute juste sur 8080
    createListeningSocket(8080);
}

void Server::stop() {
    for (size_t i = 0; i < _listenSockets.size(); ++i)
        close(_listenSockets[i]);
    _listenSockets.clear();
    _running = false;
}
