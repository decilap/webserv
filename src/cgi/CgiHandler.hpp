#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include "../common.hpp"

class CgiHandler {
public:
    struct Result {
        int exit_code;
        std::string raw;      // sortie brute CGI (headers + \r\n\r\n + body)
        bool timed_out;
        std::string error;    // message en cas d’échec
    };

    CgiHandler();
    Result run(const std::string &script_path,
               const std::string &cgi_exec_path,
               const std::map<std::string, std::string> &env,
               const std::string &request_body,
               int timeout_ms = 3000,
               size_t max_output = 4 * 1024 * 1024); // 4MB cap

private:
    int makePipe(int fds[2]);
    void closePipe(int fds[2]);
};

#endif
