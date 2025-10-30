#ifndef ERRORPAGE_HPP
#define ERRORPAGE_HPP

#include "../common.hpp"

class ErrorPage {
private:
    std::map<int, std::string> _defaultPages;

public:
    ErrorPage();

    std::string getErrorBody(int code) const;
    std::string getErrorFilePath(int code) const;
};

#endif
