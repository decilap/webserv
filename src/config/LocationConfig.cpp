#include "LocationConfig.hpp"

LocationConfig::LocationConfig()
    : autoindex(false),
      return_code(0)
{
    // Valeurs par défaut facultatives
    path = "/";
    root = "www";
    upload_path = "";
    cgi_extension = "";
    cgi_exec = "";
    return_target = "";
}