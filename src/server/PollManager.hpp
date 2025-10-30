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

#include "Client.hpp"
#include "Server.hpp"  // pour accéder aux sockets d'écoute
#include "../config/ServerConfig.hpp"
#include <map>

class PollManager {
private:
    std::vector<struct pollfd> _fds; // tous les FDs surveillés
    std::map<int, bool> _isListening; // pour savoir si un FD est un socket d'écoute
    std::map<int, Client*> _clients;
    std::map<int, ServerConfig> _socketToConfig; // map socket -> config

    bool _running;

    void acceptNewClient(int listenFd);
    void handleClientData(int clientFd);

public:
    PollManager();
    ~PollManager();

    void addListeningSockets(const std::vector<int>& sockets);
    void addServerConfig(int socket, const ServerConfig& config);
    void loop();
};

#endif
