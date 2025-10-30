#ifndef SERVER_HPP
#define SERVER_HPP

#include "../common.hpp"
#include "../config/ServerConfig.hpp"

class Server {
private:
    std::vector<int> _listenSockets;
    bool _running;

    std::vector<ServerConfig> _configs; // ✅ nouvelle donnée : la configuration du ou des serveurs

    void createListeningSocket(int port);
    void setNonBlocking(int fd);

public:
    // --- Constructeurs ---
    Server(); // par défaut (ancienne version)
    Server(const std::vector<ServerConfig> &configs); // ✅ nouveau constructeur configuré

    ~Server();

    // --- Méthodes ---
    void start();  // lance les sockets d'écoute
    void stop();   // ferme proprement
    std::vector<int> getListeningSockets() const;
    const std::vector<ServerConfig>& getConfigs() const;
};

#endif

