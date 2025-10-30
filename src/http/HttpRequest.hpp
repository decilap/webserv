#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "../common.hpp"

class HttpRequest {
private:
    std::string _method;
    std::string _uri;
    std::string _version;
    std::map<std::string, std::string> _headers;
    std::string _body;

public:
    HttpRequest();
    bool parse(const std::string &raw);
    const std::string &getMethod() const;
    const std::string &getUri() const;
    const std::string &getVersion() const;
    const std::map<std::string, std::string> &getHeaders() const;
    const std::string &getBody() const;
};

#endif
