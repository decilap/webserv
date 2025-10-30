#include "CgiHandler.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <poll.h>
#include <errno.h>
#include <cstring>
#include <iostream>
#include <signal.h>


CgiHandler::CgiHandler() {}

int CgiHandler::makePipe(int fds[2]) {
    if (pipe(fds) < 0) return -1;
    return 0;
}

void CgiHandler::closePipe(int fds[2]) {
    if (fds[0] != -1) close(fds[0]);
    if (fds[1] != -1) close(fds[1]);
}

CgiHandler::Result CgiHandler::run(const std::string &script_path,
                                   const std::string &cgi_exec_path,
                                   const std::map<std::string, std::string> &env,
                                   const std::string &request_body,
                                   int timeout_ms,
                                   size_t max_output) {
    Result r; r.exit_code = -1; r.timed_out = false;

    int inpipe[2] = {-1,-1};   // parent -> child (stdin du CGI)
    int outpipe[2] = {-1,-1};  // child -> parent (stdout du CGI)
    if (makePipe(inpipe) < 0 || makePipe(outpipe) < 0) {
        r.error = "pipe() failed";
        return r;
    }

    pid_t pid = fork();
    if (pid < 0) {
        r.error = "fork() failed";
        closePipe(inpipe); closePipe(outpipe);
        return r;
    }
    if (pid == 0) {
        // Child
        dup2(inpipe[0], STDIN_FILENO);
        dup2(outpipe[1], STDOUT_FILENO);

        closePipe(inpipe); closePipe(outpipe);

        // Construire l'environnement
        std::vector<std::string> env_kv;
        env_kv.reserve(env.size());
        for (std::map<std::string, std::string>::const_iterator it=env.begin(); it!=env.end(); ++it)
            env_kv.push_back(it->first + "=" + it->second);

        std::vector<char*> envp;
        envp.reserve(env_kv.size()+1);
        for (size_t i=0;i<env_kv.size();++i)
            envp.push_back(const_cast<char*>(env_kv[i].c_str()));
        envp.push_back(NULL);

        // Prépare argv
        std::vector<char*> argv;
        if (!cgi_exec_path.empty()) {
            argv.push_back(const_cast<char*>(cgi_exec_path.c_str()));
            argv.push_back(const_cast<char*>(script_path.c_str()));
            argv.push_back(NULL);
            execve(cgi_exec_path.c_str(), &argv[0], &envp[0]);
        } else {
            // Script exécutable avec shebang
            argv.push_back(const_cast<char*>(script_path.c_str()));
            argv.push_back(NULL);
            execve(script_path.c_str(), &argv[0], &envp[0]);
        }
        _exit(127); // execve failed
    }

    // Parent
    // On écrit request_body vers inpipe[1], on lit outpipe[0]
    close(inpipe[0]);  // lecture parent -> inutilisée
    close(outpipe[1]); // écriture enfant -> parent ferme

    // Non bloquant pour la lecture
    int flags = fcntl(outpipe[0], F_GETFL, 0);
    fcntl(outpipe[0], F_SETFL, flags | O_NONBLOCK);

    // Ecrit le body (peut être vide)
    ssize_t to_write = request_body.size();
    const char *wp = request_body.empty() ? NULL : request_body.data();
    while (to_write > 0) {
        ssize_t n = write(inpipe[1], wp, to_write);
        if (n < 0) {
            if (errno == EAGAIN || errno == EINTR) continue;
            break;
        }
        wp += n;
        to_write -= n;
    }
    // ferme stdin enfant (signale EOF)
    close(inpipe[1]);

    // Lit jusqu’au timeout ou fin
    std::string out;
    out.reserve(8192);

    int elapsed = 0;
    const int step = 50; // ms
    char buf[4096];

    while (true) {
        struct pollfd pfd;
        pfd.fd = outpipe[0];
        pfd.events = POLLIN;
        pfd.revents = 0;

        int pr = poll(&pfd, 1, step);
        if (pr > 0 && (pfd.revents & POLLIN)) {
            ssize_t n = read(outpipe[0], buf, sizeof(buf));
            if (n > 0) {
                out.append(buf, n);
                if (out.size() > max_output) { // hard cap
                    break;
                }
            } else if (n == 0) {
                // EOF
                break;
            } else if (errno != EAGAIN && errno != EINTR) {
                break;
            }
        }
        // Vérifie si le process enfant est terminé
        int status = 0;
        pid_t w = waitpid(pid, &status, WNOHANG);
        if (w == pid) {
            // enfant terminé -> continuer de vider possible stdout
            // mais si plus rien à lire, on sortira au prochain tour
        }

        elapsed += step;
        if (elapsed >= timeout_ms) {
            r.timed_out = true;
            kill(pid, SIGKILL);
            // vider pipe si dispo
            (void)read(outpipe[0], buf, sizeof(buf));
            break;
        }
    }

    close(outpipe[0]);

    // Récupère le code de sortie
    int status = 0;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) r.exit_code = WEXITSTATUS(status);
    else r.exit_code = -1;

    r.raw = out;
    return r;
}
