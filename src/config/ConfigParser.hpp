#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "../common.hpp"
#include "ServerConfig.hpp"

class ConfigParser {
private:
    std::vector<ServerConfig> _servers;

public:
    ConfigParser();
    bool parse(const std::string &path);
    const std::vector<ServerConfig> &getServers() const;

private:
    void parseServerBlock(std::ifstream &file, ServerConfig &server);
    void parseLocationBlock(std::ifstream &file, LocationConfig &location);
};

#endif
