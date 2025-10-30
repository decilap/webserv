#include "Client.hpp"
#include "../cgi/CgiHandler.hpp"
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
        return true; // not complete yet

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

        if (stat(path.c_str(), &st) < 0)
        {
            std::cerr << "[DELETE] Not found: " << path << std::endl;
            res.setStatus(404);
            res.setBodyFromFile("www/error_pages/404.html");
        }
        else if (S_ISDIR(st.st_mode))
        {
            std::cerr << "[DELETE] Target is directory: " << path << std::endl;
            res.setStatus(403);
            res.setBodyFromFile("www/error_pages/403.html");
        }
        else if (access(path.c_str(), W_OK) != 0)
        {
            std::cerr << "[DELETE] Permission denied: " << path << std::endl;
            res.setStatus(403);
            res.setBodyFromFile("www/error_pages/403.html");
        }
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

    // === [4] CGI scripts ===
    // === [4] CGI execution ===
    if (req.getMethod() == "GET" || req.getMethod() == "POST")
    {
        std::string uri = req.getUri();
        if (uri.find("/cgi-bin/") == 0)
        {
            std::string script = "www" + uri;

            HttpResponse res;
            CgiHandler cgi;
            std::map<std::string, std::string> env;

            // === Variables d’environnement minimales CGI/1.1 ===
            env["REQUEST_METHOD"] = req.getMethod();
            env["SERVER_PROTOCOL"] = "HTTP/1.1";
            env["GATEWAY_INTERFACE"] = "CGI/1.1";
            env["SERVER_SOFTWARE"] = "Webserv/1.0";
            env["SERVER_NAME"] = "127.0.0.1";
            env["SERVER_PORT"] = "8080";
            env["SCRIPT_FILENAME"] = script;
            env["SCRIPT_NAME"] = uri;
            env["PATH_INFO"] = uri;
            env["QUERY_STRING"] = "";
            env["REMOTE_ADDR"] = "127.0.0.1";
            env["REDIRECT_STATUS"] = "200"; // utile pour php-cgi

            std::map<std::string, std::string> headers = req.getHeaders();
            if (headers.find("Content-Type") != headers.end())
                env["CONTENT_TYPE"] = headers["Content-Type"];
            if (headers.find("Content-Length") != headers.end())
                env["CONTENT_LENGTH"] = headers["Content-Length"];

            std::string body = (req.getMethod() == "POST") ? req.getBody() : "";

            // === Lancement du script CGI ===
            CgiHandler::Result r = cgi.run(script, "", env, body);

            if (r.timed_out)
            {
                res.setStatus(504);
                res.setBodyString("<html><body><h1>504 Gateway Timeout</h1></body></html>");
            }
            else if (r.raw.empty())
            {
                res.setStatus(500);
                res.setBodyString("<html><body><h1>500 CGI Execution Failed</h1></body></html>");
            }
            else
            {
                // === Parsing des headers CGI ===
                std::string::size_type sep = r.raw.find("\r\n\r\n");
                std::string headersPart = (sep != std::string::npos) ? r.raw.substr(0, sep) : "";
                std::string bodyPart = (sep != std::string::npos) ? r.raw.substr(sep + 4) : r.raw;

                int status = 200;
                std::istringstream ss(headersPart);
                std::string line;

                while (std::getline(ss, line))
                {
                    if (!line.empty() && line.back() == '\r')
                        line.erase(line.size() - 1);

                    if (line.find("Status:") == 0)
                    {
                        std::string val = line.substr(7);
                        while (!val.empty() && val[0] == ' ')
                            val.erase(0, 1);
                        status = std::atoi(val.c_str());
                    }
                    else if (line.find("Content-Type:") == 0)
                    {
                        std::string val = line.substr(13);
                        while (!val.empty() && val[0] == ' ')
                            val.erase(0, 1);
                        res.setHeader("Content-Type", val);
                    }
                    else if (line.find("Location:") == 0)
                    {
                        std::string val = line.substr(9);
                        while (!val.empty() && val[0] == ' ')
                            val.erase(0, 1);
                        res.setStatus(302);
                        res.setHeader("Location", val);
                    }
                    else
                    {
                        // Tout autre header CGI → HTTP
                        std::string::size_type pos = line.find(':');
                        if (pos != std::string::npos)
                        {
                            std::string key = line.substr(0, pos);
                            std::string val = line.substr(pos + 1);
                            while (!val.empty() && val[0] == ' ')
                                val.erase(0, 1);
                            res.setHeader(key, val);
                        }
                    }
                }

                res.setStatus(status);
                res.setBodyString(bodyPart);
            }

            _bufferOut = res.build();
            _bufferIn.clear();
            _state = CLIENT_WRITE;
            return true;
        }
    }

    // === [5] GET classique ===
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
