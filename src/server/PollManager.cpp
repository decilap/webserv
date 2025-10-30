#include "PollManager.hpp"
#include "Client.hpp"
#include <netinet/in.h>   // ✅ sockaddr_in, htons, htonl
#include <sys/socket.h>   // ✅ socket(), bind(), accept()
#include <arpa/inet.h>    // ✅ inet_ntoa(), inet_addr() (optionnel)
#include <unistd.h>       // ✅ close()
#include <fcntl.h>        // ✅ fcntl()
#include <cerrno>         // ✅ errno
#include <cstring>        // ✅ strerror()

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

void PollManager::addServerConfig(int socket, const ServerConfig& config) {
    _socketToConfig[socket] = config;
}

#include "PollManager.hpp"
#include "Client.hpp"

void PollManager::acceptNewClient(int listenFd) {
    sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);
    int clientFd = accept(listenFd, (sockaddr*)&clientAddr, &len);
    if (clientFd < 0) {
        std::cerr << "[Poll] accept() failed: " << strerror(errno) << std::endl;
        return;
    }

    int flags = fcntl(clientFd, F_GETFL, 0);
    if (flags == -1)
        flags = 0;
    if (fcntl(clientFd, F_SETFL, flags | O_NONBLOCK) == -1)
        std::cerr << "[Poll] fcntl(O_NONBLOCK) failed on fd " << clientFd << std::endl;
    struct pollfd clientPfd;
    clientPfd.fd = clientFd;
    clientPfd.events = POLLIN;
    clientPfd.revents = 0;
    _fds.push_back(clientPfd);

    // Get the root from the server config for this listening socket
    std::string root = "www"; // default
    if (_socketToConfig.find(listenFd) != _socketToConfig.end()) {
        root = _socketToConfig[listenFd].root;
    }

    Client *c = new Client(clientFd, root);
    _clients[clientFd] = c;

    std::cout << "[Poll] New client connected (fd " << clientFd << ")" << std::endl;
}

void PollManager::handleClientData(int clientFd) {
    Client *client = _clients[clientFd];
    if (!client) {
        std::cerr << "[Poll] Client not found for fd " << clientFd << std::endl;
        return;
    }

    if (client->getState() == CLIENT_READ) {
        if (!client->handleRead()) {
            // Connection closed or error
            std::cout << "[Poll] Client disconnected (fd " << clientFd << ")" << std::endl;
            delete client;
            _clients.erase(clientFd);
            for (size_t i = 0; i < _fds.size(); ++i) {
                if (_fds[i].fd == clientFd) {
                    _fds.erase(_fds.begin() + i);
                    break;
                }
            }
            return;
        }
        // If state changed to WRITE, update pollfd events
        if (client->getState() == CLIENT_WRITE) {
            for (size_t i = 0; i < _fds.size(); ++i) {
                if (_fds[i].fd == clientFd) {
                    _fds[i].events = POLLOUT;
                    break;
                }
            }
        }
    }
    else if (client->getState() == CLIENT_WRITE) {
        if (!client->handleWrite()) {
            std::cout << "[Poll] Client write error (fd " << clientFd << ")" << std::endl;
            delete client;
            _clients.erase(clientFd);
            for (size_t i = 0; i < _fds.size(); ++i) {
                if (_fds[i].fd == clientFd) {
                    _fds.erase(_fds.begin() + i);
                    break;
                }
            }
            return;
        }
        // If state changed back to READ, update pollfd events
        if (client->getState() == CLIENT_READ) {
            for (size_t i = 0; i < _fds.size(); ++i) {
                if (_fds[i].fd == clientFd) {
                    _fds[i].events = POLLIN;
                    break;
                }
            }
        }
    }
    else if (client->getState() == CLIENT_CLOSE) {
        std::cout << "[Poll] Client requested close (fd " << clientFd << ")" << std::endl;
        delete client;
        _clients.erase(clientFd);
        for (size_t i = 0; i < _fds.size(); ++i) {
            if (_fds[i].fd == clientFd) {
                _fds.erase(_fds.begin() + i);
                break;
            }
        }
    }
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
            if (!(_fds[i].revents & (POLLIN | POLLOUT)))
                continue;

            int fd = _fds[i].fd;
            if (_isListening.find(fd) != _isListening.end()) {
                if (_fds[i].revents & POLLIN)
                    acceptNewClient(fd);
            }
            else {
                handleClientData(fd);
            }
        }
    }
}
