#include <csignal>
#include <cstdlib>
#include "server/Server.hpp"
#include "server/PollManager.hpp"

Server *g_srv = NULL; // pointeur global pour clean exit

void handle_sigint(int) {
    std::cout << "\n[Signal] SIGINT received — shutting down..." << std::endl;
    if (g_srv)
        g_srv->stop();
    exit(0);
}

int main() {
    signal(SIGINT, handle_sigint);

    Server srv;
    g_srv = &srv;
    srv.start();

    PollManager poll;
    poll.addListeningSockets(srv.getListeningSockets());
    poll.loop();

    return 0;
}
