//
// Created by aurailus on 2020-02-09.
//

#pragma once

#include <string>
#include <array>
#include <memory>
#include <cute_files/cute_files.h>

class VenusParser {
public:
    static std::string parse(const std::string& fileName, const std::string& fileContents);
};
