#ifndef MULTIPARTPARSER_HPP
#define MULTIPARTPARSER_HPP

#include <string>
#include <map>
#include <vector>
#include <iostream>

class MultipartParser {
private:
    std::string _boundary;
    std::string _raw;
    std::map<std::string, std::string> _files; // nom → contenu

    void extractBoundary(const std::string &contentType);
    void parseParts();

public:
    MultipartParser();
    bool parse(const std::string &raw, const std::string &contentType);
    const std::map<std::string, std::string> &getFiles() const;
};

#endif
