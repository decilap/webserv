#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <string>
#include <vector>
#include "LocationConfig.hpp"

struct ServerConfig {
    int listen_port;
    std::string server_name;
    std::string root;
    std::string index;
    size_t client_max_body_size;
    std::vector<LocationConfig> locations;

    ServerConfig();
};

#endif
