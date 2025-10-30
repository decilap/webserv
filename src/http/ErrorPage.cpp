#include "ErrorPage.hpp"

ErrorPage::ErrorPage() {
    _defaultPages[201] = "www/error_pages/201.html";
    _defaultPages[404] = "www/error_pages/404.html";
    _defaultPages[403] = "www/error_pages/403.html";
    _defaultPages[500] = "www/error_pages/500.html";
}

std::string ErrorPage::getErrorBody(int code) const {
    std::string path = getErrorFilePath(code);
    std::ifstream file(path.c_str());
    if (!file.is_open()) {
        std::ostringstream fallback;
        fallback << "<html><body><h1>Error " << code << "</h1></body></html>";
        return fallback.str();
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

std::string ErrorPage::getErrorFilePath(int code) const {
    std::map<int, std::string>::const_iterator it = _defaultPages.find(code);
    if (it != _defaultPages.end())
        return it->second;
    return "www/error_pages/500.html";
}
