//
// Created by aurailus on 2020-02-11.
//

#include <vector>
#include <sstream>
#include <algorithm>

#include "ErrorFormatter.h"

#include "../util/Log.h"

std::string ErrorFormatter::formatError(const std::string &fileName, int line, const std::string &stack, std::string file, bool ansiColors) noexcept {
    const std::string red = (ansiColors ? Log::red : "");
    const std::string unbl = (ansiColors ? Log::unbl : "");
    const std::string endl  = (ansiColors ? Log::endl : "\n");

    std::stringstream out {};

    // Split the file into lines, and add them to a vector
    std::vector<std::string> fileLines {};
    size_t pos = 0;
    std::string token;

    while ((pos = file.find("\n")) != std::string::npos) {
        token = file.substr(0, pos);
        fileLines.push_back(token);
        file.erase(0, pos + 1);
    }
    fileLines.push_back(file);

    while (fileLines.back() == "") fileLines.pop_back();

    // Format and add lines to the stringstream
    for (unsigned int i = (std::max)(0, line - 6); i < (std::min)(static_cast<int>(fileLines.size()), line + 5); i++) {
        for (unsigned int j = 0; j < 3 - std::to_string(i + 1).length(); j++) out << " ";
        out << red << (i+1 == line ? unbl : "") << (i+1) << (i+1 == line ? " # " : " | ") << fileLines[i] << endl;
    }

    // Add the stack trace at the bottom
    out << endl << red << stack << endl;

    return out.str();
}
