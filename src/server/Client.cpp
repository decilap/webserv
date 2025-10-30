#include "Client.hpp"

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
    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));
    ssize_t bytes = recv(_fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes == 0) {
        _state = CLIENT_CLOSE;
        return false; // client disconnected
    }
    else if (bytes < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            std::cerr << "[Client] recv() error: " << strerror(errno) << std::endl;
        return true;
    }

    _bufferIn.append(buffer, bytes);
    _bufferOut = _bufferIn; // pour l’instant → écho
    _bufferIn.clear();
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
