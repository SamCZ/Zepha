//
// Created by aurailus on 2020-02-09.
//

#include "VenusParser.h"

std::string VenusParser::parse(std::string src) {
    #ifdef _WIN32
        const static char* EXECUTABLE_NAME = "zepha-venus-win.exe";
    #elif __APPLE__
        const static char* EXECUTABLE_NAME = "zepha-venus-macos";
    #else
        const static char* EXECUTABLE_NAME = "zepha-venus-linux";
    #endif

    const static std::string path = "../zepha-venus/" + std::string(EXECUTABLE_NAME);
    const static bool exists = cf_file_exists(path.data());

    if (!exists) throw std::string("Trying to compile a venus file when zepha-venus has not been built!");

    std::string call = path + " " + src;

    std::array<char, 128> buffer;
    std::string result;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(call.data(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    const static std::string errorHeader = "--ZEPHA_PARSING_ERROR--";

    if (result.length() > errorHeader.length() && result.substr(0, errorHeader.length()) == errorHeader) {
        throw std::string(result.substr(errorHeader.length() + 1, result.length() - errorHeader.length() - 1));
    }

    return result;
}
