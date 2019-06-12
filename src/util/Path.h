//
// Created by aurailus on 11/06/19.
//

#ifndef ZEUS_PATH_H
#define ZEUS_PATH_H


#include <string>
#include <vector>
#include <sstream>

class Path {
public:
    explicit Path(std::string path_string);

    bool contains(Path other);

    std::string to_string();

private:
    std::vector<std::string> tokens {};
};


#endif //ZEUS_PATH_H
