#include "HttpResponse.hpp"

HttpResponse::HttpResponse() {
    _statusTexts[200] = "OK";
    _statusTexts[404] = "Not Found";
    _statusTexts[403] = "Forbidden";
    _statusTexts[500] = "Internal Server Error";
}

void HttpResponse::setStatus(int code) { _status = code; }

void HttpResponse::setHeader(const std::string &key, const std::string &value) {
    _headers[key] = value;
}

void HttpResponse::setBodyFromFile(const std::string &path) {
    struct stat st;
    if (stat(path.c_str(), &st) < 0 || S_ISDIR(st.st_mode)) {
        _status = 404;
        _body = "<html><body><h1>404 Not Found</h1></body></html>";
        return;
    }

    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
    if (!file) {
        _status = 403;
        _body = "<html><body><h1>403 Forbidden</h1></body></html>";
        return;
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    _body = ss.str();
    _status = 200;
}

std::string HttpResponse::build() const {
    std::ostringstream res;
    res << "HTTP/1.1 " << _status << " " << _statusTexts.at(_status) << "\r\n";
    res << "Content-Type: text/html\r\n";
    res << "Content-Length: " << _body.size() << "\r\n";
    res << "Connection: close\r\n\r\n";
    res << _body;
    return res.str();
}
