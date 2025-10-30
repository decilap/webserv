#include "HttpResponse.hpp"

HttpResponse::HttpResponse() : _status(0) {
    _statusTexts[200] = "OK";
    _statusTexts[201] = "Created";
    _statusTexts[404] = "Not Found";
    _statusTexts[403] = "Forbidden";
    _statusTexts[500] = "Internal Server Error";
}

void HttpResponse::setStatus(int code) { _status = code; }

void HttpResponse::setHeader(const std::string &key, const std::string &value) {
    _headers[key] = value;
}

void HttpResponse::setBody(const std::string &body) {
    _body = body;
}

void HttpResponse::setBodyFromFile(const std::string &path) {
    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        _status = 404;
        _body = _errorPage.getErrorBody(404);
        return;
    }

    try {
        std::ostringstream ss;
        ss << file.rdbuf();

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
        std::cerr << "[HttpResponse] Exception while reading file: " << e.what() << std::endl;
        _status = 500;
        _body = _errorPage.getErrorBody(500);
    }
}

void HttpResponse::setBodyFromFile(const std::string &path, const std::string &uri) {
    setBodyFromPath(path, uri);
}

void HttpResponse::setBodyFromPath(const std::string &path, const std::string &uri) {
    struct stat st;

    // Vérifier si le chemin existe
    if (stat(path.c_str(), &st) < 0) {
        _status = 404;
        _body = _errorPage.getErrorBody(404);
        return;
    }

    // 📁 Cas 1 : le chemin est un répertoire
    if (S_ISDIR(st.st_mode)) {
        std::string indexPath = path + "/index.html";

        // S'il y a un index.html, on le sert
        if (stat(indexPath.c_str(), &st) == 0 && !S_ISDIR(st.st_mode)) {
            std::ifstream indexFile(indexPath.c_str());
            if (indexFile.is_open()) {
                std::ostringstream ss;
                ss << indexFile.rdbuf();
                _body = ss.str();
                _status = 200;
                indexFile.close();
                return;
            }
        }

        // Sinon on génère l'autoindex
        _body = Autoindex::generate(path, uri);
        _status = 200;
        return;
    }

    // 📄 Cas 2 : le chemin est un fichier régulier
    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        _status = 403;
        _body = _errorPage.getErrorBody(403);
        return;
    }

    try {
        std::ostringstream ss;
        ss << file.rdbuf();

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
        std::cerr << "[HttpResponse] Exception while reading file: " << e.what() << std::endl;
        _status = 500;
        _body = _errorPage.getErrorBody(500);
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
