#include "Server.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>

Server::Server() : _running(false) {}

Server::Server(const std::vector<ServerConfig> &configs)
    : _running(false), _configs(configs)
{
    std::cout << "[Server] Loaded " << _configs.size() << " server config(s)" << std::endl;
}

Server::~Server() {
    stop();
}

void Server::setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        flags = 0;
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
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
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "bind() failed on port " << port << ": " << strerror(errno) << std::endl;
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

    if (_configs.empty()) {
        // Ancien comportement (fallback)
        createListeningSocket(8080);
        return;
    }

    // --- Nouveau comportement : configuration dynamique ---
    for (size_t i = 0; i < _configs.size(); ++i) {
        int port = _configs[i].listen_port;
        if (port <= 0)
            port = 8080;
        createListeningSocket(port);
    }
}

void Server::stop() {
    for (size_t i = 0; i < _listenSockets.size(); ++i)
        close(_listenSockets[i]);
    _listenSockets.clear();
    _running = false;
}

std::vector<int> Server::getListeningSockets() const {
    return _listenSockets;
}

const std::vector<ServerConfig>& Server::getConfigs() const {
    return _configs;
}
