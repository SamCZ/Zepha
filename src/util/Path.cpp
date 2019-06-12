//
// Created by aurailus on 11/06/19.
//

#include "Path.h"

Path::Path(std::string path_string) {
    do {
        unsigned long end = std::min(path_string.find('/'), path_string.find('\\'));
        if (end == std::string::npos) {
            tokens.push_back(path_string);
            path_string = "";
        }
        else {
            auto token = path_string.substr(0, end);
            if (token == "..") { if (!tokens.empty()) tokens.pop_back(); }
            else if (!token.empty() && token != ".") tokens.push_back(std::move(token));

            path_string = path_string.substr(end + 1, path_string.length() - end - 1);
        }
    }
    while (!path_string.empty());
}

std::string Path::to_string() {
    std::stringstream s;
    s << "/";
    for (const auto &i : tokens) s << i << "/";
    return s.str();
}

bool Path::contains(Path other) {
    std::string sMe = to_string();
    std::string sOther = other.to_string();
    return (sOther.substr(0, sMe.length()) == sMe);
}
