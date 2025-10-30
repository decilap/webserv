#include "common.hpp"
#include "server/Server.hpp"
#include "config/ConfigParser.hpp"
#include "server/PollManager.hpp"

Server *g_srv = NULL;

void handle_sigint(int) {
    std::cout << "\n[Signal] SIGINT received — shutting down..." << std::endl;
    if (g_srv)
        g_srv->stop();
    exit(0);
}

int main(int argc, char **argv) {
    // --- 1. Gestion du signal Ctrl+C ---
    signal(SIGINT, handle_sigint);

    // --- 2. Lecture du fichier de configuration ---
    std::string configPath = (argc > 1) ? argv[1] : "conf/webserv.conf";

    ConfigParser parser;
    if (!parser.parse(configPath)) {
        std::cerr << "[Error] Failed to parse configuration file: " << configPath << std::endl;
        return 1;
    }

    const std::vector<ServerConfig> &servers = parser.getServers();
    if (servers.empty()) {
        std::cerr << "[Error] No valid server block in configuration." << std::endl;
        return 1;
    }

    // --- 3. Démarrage du serveur principal ---
    Server srv(servers);
    g_srv = &srv;
    srv.start();

    // --- 4. Boucle principale ---
    PollManager poll;
    std::vector<int> sockets = srv.getListeningSockets();
    const std::vector<ServerConfig>& configs = srv.getConfigs();

    poll.addListeningSockets(sockets);

    // Associate each socket with its server config
    for (size_t i = 0; i < sockets.size() && i < configs.size(); ++i) {
        poll.addServerConfig(sockets[i], configs[i]);
    }

    poll.loop();

    return 0;
}
