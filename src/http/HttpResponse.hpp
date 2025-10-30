#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include "ErrorPage.hpp"
#include "Autoindex.hpp"

class HttpResponse {
private:
    int _status;
    std::map<int, std::string> _statusTexts;
    std::map<std::string, std::string> _headers;
    std::string _body;
    ErrorPage _errorPage;


public:
    HttpResponse();
    void setStatus(int code);
    void setHeader(const std::string &key, const std::string &value);
    void setBody(const std::string &body);
    void setBodyFromFile(const std::string &path);
    void setBodyFromFile(const std::string &path, const std::string &uri);
    void setBodyFromPath(const std::string &path, const std::string &uri);
    std::string build() const;
};

#endif
