#ifndef POLLMANAGER_HPP
#define POLLMANAGER_HPP

#include "../common.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "../config/ServerConfig.hpp"

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
    void stop();
};

#endif
