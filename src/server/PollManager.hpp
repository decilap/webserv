#ifndef POLLMANAGER_HPP
#define POLLMANAGER_HPP

#include <vector>
#include <map>
#include <iostream>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>

#include "Server.hpp"  // pour accéder aux sockets d’écoute

class PollManager {
private:
    std::vector<struct pollfd> _fds; // tous les FDs surveillés
    std::map<int, bool> _isListening; // pour savoir si un FD est un socket d’écoute

    bool _running;

    void acceptNewClient(int listenFd);
    void handleClientData(int clientFd);

public:
    PollManager();
    ~PollManager();

    void addListeningSockets(const std::vector<int>& sockets);
    void loop();
};

#endif
