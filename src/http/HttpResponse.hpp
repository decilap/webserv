#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "../common.hpp"
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
    void setBodyString(const std::string &body);      // 👈 ajout pour réponse custom
    void setBodyFromFile(const std::string &path);    // fichier simple
    void setBodyFromFile(const std::string &path, const std::string &uri); // fichiers + autoindex
    std::string build() const;
};

#endif
