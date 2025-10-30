#include "ServerConfig.hpp"

ServerConfig::ServerConfig()
    : listen_port(8080),
      client_max_body_size(1048576) // 1MB par défaut
{}