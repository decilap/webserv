#ifndef SERVER_HPP
#define SERVER_HPP

#include "../common.hpp"
#include "../config/ServerConfig.hpp"

class Server {
private:
    std::vector<int> _listenSockets;
    bool _running;

    std::vector<ServerConfig> _configs;

    void createListeningSocket(int port);
    void setNonBlocking(int fd);

public:
    Server();
    Server(const std::vector<ServerConfig> &configs);

    ~Server();

    // --- Méthodes ---
    void start();  // lance les sockets d'écoute
    void stop();   // ferme proprement
    const std::vector<int>& getListeningSockets() const;
    const std::vector<ServerConfig>& getConfigs() const;
};

#endif

