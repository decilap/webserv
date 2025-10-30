#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <cstring>      // memset
#include <cerrno>
#include <unistd.h>     // close
#include <fcntl.h>      // fcntl
#include <netinet/in.h> // sockaddr_in
#include <sys/socket.h> // socket, bind, listen

class Server {
private:
    std::vector<int> _listenSockets;
    bool _running;

    void createListeningSocket(int port);
    void setNonBlocking(int fd);

public:
    Server();
    ~Server();

    void start();  // lance les sockets d’écoute
    void stop();   // ferme proprement
    std::vector<int> getListeningSockets() const;

};

#endif
