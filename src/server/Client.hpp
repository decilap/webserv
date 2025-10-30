#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <fcntl.h>

enum ClientState {
    CLIENT_READ,
    CLIENT_WRITE,
    CLIENT_CLOSE
};

class Client {
private:
    int _fd;
    ClientState _state;
    std::string _bufferIn;
    std::string _bufferOut;

public:
    Client(int fd);
    ~Client();

    int getFd() const;
    ClientState getState() const;
    void setState(ClientState s);

    bool handleRead();
    bool handleWrite();
    void closeConnection();
};

#endif
