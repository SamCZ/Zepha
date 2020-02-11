//
// Created by aurailus on 2020-02-09.
//

#include "VenusParser.h"
#include "ErrorFormatter.h"

namespace {
#ifdef _WIN32
    const static char* EXECUTABLE_NAME = "zepha-venus-win.exe";
#elif __APPLE__
    const static char* EXECUTABLE_NAME = "zepha-venus-macos";
#else
    const static char* EXECUTABLE_NAME = "zepha-venus-linux";
#endif
}

std::string VenusParser::parse(const std::string& fileName, const std::string& fileContents) {
    const static std::string path = "../zepha-venus/" + std::string(EXECUTABLE_NAME);

    const static bool exists = cf_file_exists(path.data());
    if (!exists) throw std::runtime_error("Trying to compile a venus file when zepha-venus has not been built!");

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen((path + " " + fileName).data(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");

    std::string result;
    std::array<char, 128> buffer;
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) result += buffer.data();

    const static std::string errorHeader = "--ZEPHA_PARSING_ERROR--";
    if (result.length() < errorHeader.length() || result.substr(0, errorHeader.length()) != errorHeader) return result;

    // There was an error in the node app, this is it:
    std::string error = result.substr(errorHeader.length() + 1, result.length() - errorHeader.length() - 1);

    std::string::size_type lineNumStart = error.find('[');
    assert(lineNumStart != std::string::npos);
    std::string::size_type lineNumEnd = error.find_first_of(":]", lineNumStart + 1);
    assert(lineNumEnd != std::string::npos);

    int lineNum = std::stoi(error.substr(lineNumStart + 1, lineNumEnd - lineNumStart - 1));

    throw std::runtime_error(ErrorFormatter::formatError(fileName, lineNum,
            "Encountered an error compiling " + fileName + ": \n" + error, fileContents).data());
}
