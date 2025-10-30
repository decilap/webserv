#include "Client.hpp"
#include "../http/HttpRequest.hpp"
#include "../http/HttpResponse.hpp"
#include <fstream>

Client::Client(int fd) : _fd(fd), _state(CLIENT_READ) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        flags = 0;
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    std::cout << "[Client] Created client fd=" << fd << std::endl;
}

Client::~Client() {
    closeConnection();
}

int Client::getFd() const { return _fd; }
ClientState Client::getState() const { return _state; }
void Client::setState(ClientState s) { _state = s; }
bool Client::handleRead() {
    char buffer[2048];
    std::memset(buffer, 0, sizeof(buffer));
    ssize_t bytes = recv(_fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes <= 0) {
        _state = CLIENT_CLOSE;
        return false;
    }

    std::string rawRequest(buffer, bytes);
    std::cout << "[Client] Received request:\n" << rawRequest << std::endl;

    HttpRequest req;
    if (!req.parse(rawRequest)) {
        std::cerr << "[Client] Failed to parse request" << std::endl;
        _state = CLIENT_CLOSE;
        return false;
    }

    std::string uri = req.getUri();
    std::string path = "www";

    // Si l'URI est "/" ou vide, servir index.html
    if (uri == "/" || uri.empty())
        path += "/index.html";
    else
        path += uri;

    std::cout << "[Client] Requested URI: " << uri << " -> File: " << path << std::endl;

    HttpResponse res;
    res.setBodyFromFile(path);

    _bufferOut = res.build();
    _state = CLIENT_WRITE;
    return true;
}

bool Client::handleWrite() {
    if (_bufferOut.empty())
        return true;

    ssize_t bytes = send(_fd, _bufferOut.c_str(), _bufferOut.size(), 0);
    if (bytes < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            std::cerr << "[Client] send() error: " << strerror(errno) << std::endl;
        return true;
    }

    _bufferOut.erase(0, bytes);
    if (_bufferOut.empty())
        _state = CLIENT_READ;

    return true;
}

void Client::closeConnection() {
    if (_fd != -1) {
        std::cout << "[Client] Closing fd " << _fd << std::endl;
        close(_fd);
        _fd = -1;
    }
}
