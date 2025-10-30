#include "Client.hpp"
#include "../http/HttpRequest.hpp"
#include "../http/HttpResponse.hpp"
#include "../http/MultipartParser.hpp"

#include <fstream>

Client::Client(int fd) : _fd(fd), _state(CLIENT_READ)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        flags = 0;
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    std::cout << "[Client] Created client fd=" << fd << std::endl;
}

Client::~Client()
{
    closeConnection();
}

int Client::getFd() const { return _fd; }
ClientState Client::getState() const { return _state; }
void Client::setState(ClientState s) { _state = s; }

bool Client::handleWrite()
{
    if (_bufferOut.empty())
        return true;

    ssize_t bytes = send(_fd, _bufferOut.c_str(), _bufferOut.size(), 0);
    if (bytes < 0)
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            std::cerr << "[Client] send() error: " << strerror(errno) << std::endl;
        return true;
    }

    _bufferOut.erase(0, bytes);
    if (_bufferOut.empty())
        _state = CLIENT_READ;

    return true;
}

void Client::closeConnection()
{
    if (_fd != -1)
    {
        std::cout << "[Client] Closing fd " << _fd << std::endl;
        close(_fd);
        _fd = -1;
    }
}

bool Client::handleRead()
{
    char buffer[8192];
    std::memset(buffer, 0, sizeof(buffer));
    ssize_t bytes = recv(_fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes <= 0)
    {
        _state = CLIENT_CLOSE;
        return false;
    }

    // Accumulate received data
    _bufferIn.append(buffer, bytes);

    // Try to parse the request
    HttpRequest req;
    if (!req.parse(_bufferIn))
    {
        // Not enough data yet, keep reading
        return true;
    }

    // --- Vérification POST complet ---
    if (req.getMethod() == "POST")
    {
        std::map<std::string, std::string>::const_iterator it = req.getHeaders().find("Content-Length");
        if (it != req.getHeaders().end())
        {
            size_t content_length = 0;
            std::istringstream(it->second) >> content_length;
            if (req.getBody().size() < content_length)
                return true; // pas encore tout reçu
        }
    }

    // === [1] Méthode DELETE ===
    if (req.getMethod() == "DELETE")
    {
        std::string uri = req.getUri();
        std::string path = "www" + uri;

        HttpResponse res;
        struct stat st;

        // Vérifie si le fichier existe
        if (stat(path.c_str(), &st) < 0)
        {
            std::cerr << "[DELETE] Not found: " << path << std::endl;
            res.setStatus(404);
            res.setBodyFromFile("www/error_pages/404.html");
        }
        // Refuse si c’est un dossier
        else if (S_ISDIR(st.st_mode))
        {
            std::cerr << "[DELETE] Target is directory: " << path << std::endl;
            res.setStatus(403);
            res.setBodyFromFile("www/error_pages/403.html");
        }
        // Vérifie les permissions d’écriture sur le fichier
        else if (access(path.c_str(), W_OK) != 0)
        {
            std::cerr << "[DELETE] Permission denied: " << path << std::endl;
            res.setStatus(403);
            res.setBodyFromFile("www/error_pages/403.html");
        }
        // Supprime le fichier si tout est OK
        else
        {
            if (remove(path.c_str()) == 0)
            {
                std::cout << "[DELETE] File removed: " << path << std::endl;
                res.setStatus(200);
                res.setBodyString(
                    "<html><body style='background-color:#1e1e1e;color:white;text-align:center;'>"
                    "<h1>200 - File Deleted</h1>"
                    "<p>File successfully deleted from Webserv42.</p>"
                    "</body></html>");
            }
            else
            {
                std::perror("[DELETE] remove() failed");
                res.setStatus(500);
                res.setBodyFromFile("www/error_pages/500.html");
            }
        }

        _bufferOut = res.build();
        _bufferIn.clear();
        _state = CLIENT_WRITE;
        return true;
    }

    // === [2] Redirection simple (exemple statique) ===
    if (req.getUri() == "/oldpage")
    {
        HttpResponse res;
        res.setStatus(301);
        res.setHeader("Location", "/newpage");
        res.setBodyString("<html><body><h1>301 Moved Permanently</h1>"
                          "<p>Resource moved to <a href=\"/newpage\">/newpage</a></p>"
                          "</body></html>");
        _bufferOut = res.build();
        _bufferIn.clear();
        _state = CLIENT_WRITE;
        return true;
    }

    // === [3] Upload multipart (déjà OK) ===
    if (req.getMethod() == "POST")
    {
        std::map<std::string, std::string>::const_iterator it = req.getHeaders().find("Content-Type");
        if (it != req.getHeaders().end() && it->second.find("multipart/form-data") != std::string::npos)
        {
            MultipartParser parser;
            if (parser.parse(_bufferIn, it->second))
            {
                const std::map<std::string, std::string> &files = parser.getFiles();
                for (std::map<std::string, std::string>::const_iterator f = files.begin(); f != files.end(); ++f)
                {
                    std::string dest = "www/upload/" + f->first;
                    std::ofstream out(dest.c_str(), std::ios::binary);
                    if (!out.is_open())
                    {
                        std::cerr << "[Upload] Cannot write file " << dest << std::endl;
                        continue;
                    }
                    out.write(f->second.c_str(), f->second.size());
                    out.close();
                    std::cout << "[Upload] File saved: " << dest << " (" << f->second.size() << " bytes)" << std::endl;
                }

                HttpResponse res;
                res.setStatus(303);
                res.setHeader("Location", "/upload/");
                res.setBodyString("<html><body><h1>Upload success</h1>"
                                  "<p>Redirecting to <a href=\"/upload/\">/upload/</a></p>"
                                  "<script>setTimeout(function(){window.location='/upload/';},800);</script>"
                                  "</body></html>");
                _bufferOut = res.build();
                _bufferIn.clear();
                _state = CLIENT_WRITE;
                return true;
            }
        }
    }

    // === [4] Sinon : GET classique ===
    HttpResponse res;
    std::string uri = req.getUri();
    std::string path = "www" + uri;

    if (path == "www/")
        path = "www/index.html";

    res.setBodyFromFile(path, uri);
    _bufferOut = res.build();
    _bufferIn.clear();
    _state = CLIENT_WRITE;
    return true;
}
