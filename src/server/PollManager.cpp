#include "PollManager.hpp"

PollManager::PollManager() : _running(false) {}
PollManager::~PollManager() {
    for (size_t i = 0; i < _fds.size(); ++i)
        close(_fds[i].fd);
}

void PollManager::addListeningSockets(const std::vector<int>& sockets) {
    for (size_t i = 0; i < sockets.size(); ++i) {
        struct pollfd pfd;
        pfd.fd = sockets[i];
        pfd.events = POLLIN;
        pfd.revents = 0;
        _fds.push_back(pfd);
        _isListening[sockets[i]] = true;
    }
}

void PollManager::acceptNewClient(int listenFd) {
    sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);
    int clientFd = accept(listenFd, (sockaddr*)&clientAddr, &len);
    if (clientFd < 0) {
        std::cerr << "[Poll] accept() failed: " << strerror(errno) << std::endl;
        return;
    }

    int flags = fcntl(clientFd, F_GETFL, 0);
    fcntl(clientFd, F_SETFL, flags | O_NONBLOCK);

    struct pollfd clientPfd;
    clientPfd.fd = clientFd;
    clientPfd.events = POLLIN;
    clientPfd.revents = 0;
    _fds.push_back(clientPfd);

    std::cout << "[Poll] New client connected (fd " << clientFd << ")" << std::endl;
}

void PollManager::handleClientData(int clientFd) {
    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));
    ssize_t bytes = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

    if (bytes == 0) {
        std::cout << "[Poll] Client disconnected (fd " << clientFd << ")" << std::endl;
        close(clientFd);
        for (size_t i = 0; i < _fds.size(); ++i) {
            if (_fds[i].fd == clientFd) {
                _fds.erase(_fds.begin() + i);
                break;
            }
        }
        return;
    }
    else if (bytes < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            std::cerr << "[Poll] recv() failed: " << strerror(errno) << std::endl;
        return;
    }

    // Echo simple pour test
    send(clientFd, buffer, bytes, 0);
}

void PollManager::loop() {
    _running = true;
    std::cout << "[Poll] Starting event loop..." << std::endl;

    while (_running) {
        int ret = poll(&_fds[0], _fds.size(), 1000); // timeout 1s
        if (ret < 0) {
            if (errno == EINTR) continue;
            std::cerr << "[Poll] poll() error: " << strerror(errno) << std::endl;
            break;
        }
        else if (ret == 0) {
            // Timeout → rien à faire
            continue;
        }

        for (size_t i = 0; i < _fds.size(); ++i) {
            if (!(_fds[i].revents & POLLIN))
                continue;

            int fd = _fds[i].fd;
            if (_isListening.find(fd) != _isListening.end())
                acceptNewClient(fd);
            else
                handleClientData(fd);
        }
    }
}
