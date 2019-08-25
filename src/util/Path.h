//
// Created by aurailus on 11/06/19.
//

#pragma once


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

