#include "HttpRequest.hpp"

HttpRequest::HttpRequest() {}

bool HttpRequest::parse(const std::string &raw) {
    // Find the end of headers (double CRLF)
    size_t header_end = raw.find("\r\n\r\n");
    if (header_end == std::string::npos)
        return false;

    // Parse request line and headers
    std::string header_section = raw.substr(0, header_end);
    std::istringstream stream(header_section);
    std::string line;

    // Première ligne : "GET /index.html HTTP/1.1"
    if (!std::getline(stream, line))
        return false;

    std::istringstream start(line);
    if (!(start >> _method >> _uri >> _version))
        return false;

    // En-têtes
    while (std::getline(stream, line) && line != "\r") {
        size_t colon = line.find(':');
        if (colon == std::string::npos)
            continue;
        std::string key = line.substr(0, colon);
        std::string value = line.substr(colon + 1);
        if (!value.empty() && value[0] == ' ')
            value.erase(0, 1);
        if (!key.empty() && key[key.size() - 1] == '\r')
            key.erase(key.size() - 1);
        if (!value.empty() && value[value.size() - 1] == '\r')
            value.erase(value.size() - 1);
        _headers[key] = value;
    }

    // Corps - preserve binary data as-is
    _body = raw.substr(header_end + 4);

    return true;
}

const std::string &HttpRequest::getMethod() const { return _method; }
const std::string &HttpRequest::getUri() const { return _uri; }
const std::string &HttpRequest::getVersion() const { return _version; }
const std::map<std::string, std::string> &HttpRequest::getHeaders() const { return _headers; }
const std::string &HttpRequest::getBody() const { return _body; }
