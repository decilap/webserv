#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#include <string>
#include <sstream>
#include <dirent.h>
#include <sys/stat.h>
#include <ctime>
#include <iomanip>
#include <iostream>

class Autoindex {
public:
    static std::string generate(const std::string &path, const std::string &uri);
};

#endif
