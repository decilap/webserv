#include "MultipartParser.hpp"

MultipartParser::MultipartParser() {}

void MultipartParser::extractBoundary(const std::string &contentType) {
    size_t pos = contentType.find("boundary=");
    if (pos == std::string::npos)
        return;
    _boundary = "--" + contentType.substr(pos + 9);
}

bool MultipartParser::parse(const std::string &raw, const std::string &contentType) {
    _raw = raw;
    extractBoundary(contentType);
    if (_boundary.empty())
        return false;

    parseParts();
    return !_files.empty();
}

void MultipartParser::parseParts() {
    size_t pos = 0;
    while ((pos = _raw.find(_boundary, pos)) != std::string::npos) {
        size_t start = _raw.find("\r\n\r\n", pos);
        if (start == std::string::npos)
            break;
        start += 4;
        size_t end = _raw.find(_boundary, start);
        if (end == std::string::npos)
            break;

        std::string headerPart = _raw.substr(pos, start - pos);
        std::string body = _raw.substr(start, end - start);
        if (body.size() > 2 && body.substr(body.size() - 2) == "\r\n")
            body.erase(body.size() - 2);

        // Extraction du nom du fichier
        size_t fn = headerPart.find("filename=\"");
        if (fn != std::string::npos) {
            fn += 10;
            size_t fn_end = headerPart.find("\"", fn);
            std::string filename = headerPart.substr(fn, fn_end - fn);
            _files[filename] = body;
        }
        pos = end + _boundary.size();
    }
}

const std::map<std::string, std::string> &MultipartParser::getFiles() const {
    return _files;
}
