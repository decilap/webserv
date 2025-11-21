#include "common.hpp"
#include "server/Server.hpp"
#include "config/ConfigParser.hpp"
#include "server/PollManager.hpp"

Server *g_srv = NULL;
PollManager *g_poll = NULL;

void handle_sigint(int) {
    std::cout << "\n[Signal] SIGINT received — shutting down..." << std::endl;
    if (g_poll)
        g_poll->stop();
    if (g_srv)
        g_srv->stop();
}

int main(int argc, char **argv) {
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);

    // --- 2. Lecture du fichier de configuration ---
    ConfigParser *parser = new ConfigParser();
    if (!parser->parse((argc > 1) ? argv[1] : "conf/webserv.conf")) {
        std::cerr << "[Error] Failed to parse configuration file" << std::endl;
        delete parser;
        return 1;
    }

    const std::vector<ServerConfig> &servers = parser->getServers();
    if (servers.empty()) {
        std::cerr << "[Error] No valid server block in configuration." << std::endl;
        delete parser;
        return 1;
    }

    // --- 3. Démarrage du serveur principal ---
    Server *srv = new Server(servers);
    g_srv = srv;
    srv->start();

    // Parser no longer needed after server initialization
    delete parser;
    parser = NULL;

    // --- 4. Boucle principale ---
    PollManager *poll = new PollManager();
    g_poll = poll;

    // Avoid temporary vector copy - use const reference directly
    {
        const std::vector<int>& sockets = srv->getListeningSockets();
        const std::vector<ServerConfig>& configs = srv->getConfigs();

        poll->addListeningSockets(sockets);

        // Associate each socket with its server config
        for (size_t i = 0; i < sockets.size() && i < configs.size(); ++i) {
            poll->addServerConfig(sockets[i], configs[i]);
        }
    }

    poll->loop();

    // Explicit cleanup before exit
    delete poll;
    g_poll = NULL;

    delete srv;
    g_srv = NULL;

    std::cout << "[Main] Cleanup complete. Exiting." << std::endl;
    return 0;
}
