#ifndef LOCATION_CONFIG_HPP
#define LOCATION_CONFIG_HPP

#include <string>
#include <vector>

struct LocationConfig {
    std::string path;               // ex: /upload
    std::string root;               // ex: www/upload
    bool autoindex;                 // ex: on/off
    std::string upload_path;        // chemin upload
    std::string cgi_extension;      // .py, .php, ...
    std::string cgi_exec;           // /usr/bin/python3
    int return_code;                // ex: 301
    std::string return_target;      // ex: /newpage
    std::vector<std::string> allow_methods; // ex: GET, POST, DELETE

    LocationConfig();
};

#endif
