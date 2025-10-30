#include "HttpResponse.hpp"
#include "Autoindex.hpp"

HttpResponse::HttpResponse()
{
    _statusTexts[200] = "OK";
    _statusTexts[201] = "Created";
    _statusTexts[204] = "No Content";
    _statusTexts[301] = "Moved Permanently";
    _statusTexts[302] = "Found";
    _statusTexts[303] = "See Other";
    _statusTexts[400] = "Bad Request";
    _statusTexts[403] = "Forbidden";
    _statusTexts[404] = "Not Found";
    _statusTexts[500] = "Internal Server Error";
    _status = 0;
}

void HttpResponse::setStatus(int code) { _status = code; }
void HttpResponse::setHeader(const std::string &key, const std::string &value)
{
    _headers[key] = value;
}
void HttpResponse::setBody(const std::string &body) { _body = body; }
void HttpResponse::setBodyString(const std::string &body) { _body = body; }

// Fichier simple uniquement
void HttpResponse::setBodyFromFile(const std::string &path)
{
    struct stat st;
    if (stat(path.c_str(), &st) < 0 || S_ISDIR(st.st_mode))
    {
        _status = 404;
        _body = _errorPage.getErrorBody(404);
        return;
    }

    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        _status = 403;
        _body = _errorPage.getErrorBody(403);
        return;
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    _body = ss.str();
    if (_status == 0)
        _status = 200;
    file.close();
}

// Fichier ou dossier (autoindex)
void HttpResponse::setBodyFromFile(const std::string &path, const std::string &uri)
{
    struct stat st;
    if (stat(path.c_str(), &st) < 0)
    {
        _status = 404;
        _body = _errorPage.getErrorBody(404);
        return;
    }

    if (S_ISDIR(st.st_mode))
    {
        std::string indexPath = path + "/index.html";
        if (stat(indexPath.c_str(), &st) == 0 && !S_ISDIR(st.st_mode))
        {
            std::ifstream file(indexPath.c_str());
            if (file.is_open())
            {
                std::ostringstream ss;
                ss << file.rdbuf();
                _body = ss.str();
                _status = 200;
                file.close();
                return;
            }
        }

        // Aucun index -> autoindex
        _body = Autoindex::generate(path, uri);
        _status = 200;
        return;
    }

    // Sinon, fichier normal
    setBodyFromFile(path);
}

std::string HttpResponse::build() const
{
    std::ostringstream res;
    int statusCode = _status ? _status : 200;
    std::string statusText = _statusTexts.count(statusCode) ? _statusTexts.at(statusCode) : "OK";

    res << "HTTP/1.1 " << statusCode << " " << statusText << "\r\n";

    // Headers par défaut
    if (_headers.find("Content-Type") == _headers.end())
        res << "Content-Type: text/html\r\n";
    res << "Content-Length: " << _body.size() << "\r\n";
    res << "Connection: close\r\n";

    // Headers personnalisés
    for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
        res << it->first << ": " << it->second << "\r\n";

    res << "\r\n"
        << _body;
    return res.str();
}
