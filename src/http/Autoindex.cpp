#include "Autoindex.hpp"

std::string Autoindex::generate(const std::string &path, const std::string &uri) {
    DIR *dir = opendir(path.c_str());
    if (!dir)
        return "<html><body><h1>403 Forbidden</h1></body></html>";

    std::ostringstream html;
    html << "<html><head><title>Index of " << uri << "</title>\n";
    html << "<style>body{font-family:monospace;background:#1e1e1e;color:#ddd;}a{color:#6cf;text-decoration:none;}a:hover{text-decoration:underline;}td{padding:4px;}</style>\n";
    html << "</head><body>\n<h1>Index of " << uri << "</h1><hr><table>\n";
    html << "<tr><th>Name</th><th>Size</th><th>Last Modified</th></tr>\n";

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string name = entry->d_name;
        if (name == ".") continue;

        std::string fullPath = path + "/" + name;
        struct stat st;
        if (stat(fullPath.c_str(), &st) < 0)
            continue;

        // Date de modification
        char timebuf[64];
        struct tm *tm_info = localtime(&st.st_mtime);
        strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M", tm_info);

        html << "<tr><td><a href=\"" << uri;
        if (uri[uri.size()-1] != '/') html << "/";
        html << name;
        if (S_ISDIR(st.st_mode)) html << "/";
        html << "\">" << name << "</a></td>";

        if (S_ISDIR(st.st_mode))
            html << "<td>DIR</td>";
        else
            html << "<td>" << st.st_size << " bytes</td>";

        html << "<td>" << timebuf << "</td></tr>\n";
    }

    html << "</table><hr></body></html>\n";
    closedir(dir);
    return html.str();
}
