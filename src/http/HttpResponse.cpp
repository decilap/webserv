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

    // Vérifier si le fichier existe
    if (stat(path.c_str(), &st) < 0 || S_ISDIR(st.st_mode)) {
        _status = 404;
        _body = _errorPage.getErrorBody(404);
        return;
    }

    // Vérifier si le fichier peut être ouvert
    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        _status = 403;
        _body = _errorPage.getErrorBody(403);
        return;
    }

    try {
        // Lire le contenu du fichier
        std::ostringstream ss;
        ss << file.rdbuf();

        // Vérifier si la lecture a échoué
        if (file.fail() && !file.eof()) {
            std::cerr << "[HttpResponse] Error reading file: " << path << std::endl;
            _status = 500;
            _body = _errorPage.getErrorBody(500);
            file.close();
            return;
        }

        _body = ss.str();
        _status = 200;
        file.close();
    } catch (const std::exception &e) {
        // Gérer toute exception lors de la lecture
        std::cerr << "[HttpResponse] Exception while reading file: " << e.what() << std::endl;
        _status = 500;
        _body = _errorPage.getErrorBody(500);
        file.close();
    }
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
