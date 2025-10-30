#include "server/Server.hpp"

int main() {
    Server srv;
    srv.start();
    while (true) {
        pause(); // garde le process vivant pour tester le socket
    }
    return 0;
}
