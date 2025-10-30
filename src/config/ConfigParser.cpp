#include "ConfigParser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

// Helper function to remove trailing semicolon
static std::string trimSemicolon(const std::string& str) {
    std::string result = str;
    if (!result.empty() && result[result.size() - 1] == ';')
        result.erase(result.size() - 1);
    return result;
}

ConfigParser::ConfigParser() {}

bool ConfigParser::parse(const std::string &path) {
    std::ifstream file(path.c_str());
    if (!file.is_open()) {
        std::cerr << "[Config] Cannot open file: " << path << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.find("server") != std::string::npos) {
            ServerConfig srv;
            parseServerBlock(file, srv);
            _servers.push_back(srv);
        }
    }

    return !_servers.empty();
}

void ConfigParser::parseServerBlock(std::ifstream &file, ServerConfig &srv) {
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("}") != std::string::npos)
            break;

        std::istringstream iss(line);
        std::string key, value;
        iss >> key >> value;
        value = trimSemicolon(value);

        if (key == "listen")
            srv.listen_port = std::atoi(value.c_str());
        else if (key == "server_name")
            srv.server_name = value;
        else if (key == "root")
            srv.root = value;
        else if (key == "index")
            srv.index = value;
        else if (key == "client_max_body_size") {
            size_t num = std::atoi(value.c_str());
            if (value.find('M') != std::string::npos)
                num *= 1024 * 1024;
            srv.client_max_body_size = num;
        }
        else if (key == "location") {
            LocationConfig loc;
            iss >> loc.path;
            parseLocationBlock(file, loc);
            srv.locations.push_back(loc);
        }
    }
}

void ConfigParser::parseLocationBlock(std::ifstream &file, LocationConfig &loc) {
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("}") != std::string::npos)
            break;

        std::istringstream iss(line);
        std::string key, value;
        iss >> key >> value;
        value = trimSemicolon(value);

        if (key == "root")
            loc.root = value;
        else if (key == "autoindex")
            loc.autoindex = (value == "on");
        else if (key == "upload_path")
            loc.upload_path = value;
        else if (key == "cgi") {
            iss >> loc.cgi_extension >> loc.cgi_exec;
        }
        else if (key == "return") {
            iss >> loc.return_code >> loc.return_target;
        }
        else if (key == "allow_methods") {
            std::string method;
            while (iss >> method)
                loc.allow_methods.push_back(method);
        }
    }
}

const std::vector<ServerConfig> &ConfigParser::getServers() const {
    return _servers;
}
