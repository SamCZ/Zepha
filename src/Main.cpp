#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define STB_IMAGE_IMPLEMENTATION
#define CUTE_FILES_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "game/Client.h"
#include "server/Server.h"
#include "util/Log.h"

uint64_t constexpr mix(char m, uint64_t s) {
    return ((s<<7) + ~(s>>3)) + ~m;
}
uint64_t constexpr hashStr(const char * m) {
    return (*m) ? mix(*m, hashStr(m+1)) : 0;
}

int main(int argc, char* argv[]) {
    Address addr {"127.0.0.1", 32000};

    enum class Mode {
        NONE, CLIENT, SERVER, LOCAL_SERVER
    };

    //Collect arguments into `args` map
    std::map<std::string, std::string> args;
    for (int i = 1; i < argc; i++) {

        std::string arg(argv[i]);
        size_t equals = arg.find('=');
        std::string first = (equals == -1) ? arg : arg.substr(0, equals);

        if (args.count(first)) {
            std::cout << Log::err << "Duplicate argument " << first << "." << Log::endl;
            return -1;
        }
        if (equals == -1) args.emplace(first, "");
        else {
            if (equals == arg.length() - 1) {
                std::cout << Log::err << "Empty equals-assignment " << first << "." << Log::endl;
                return -1;
            }
            args.emplace(first, arg.substr(equals + 1, arg.length()));
        }
    }

    Mode mode = Mode::CLIENT;

    //Parse the arguments map
    for (auto arg : args) {
        switch (hashStr(arg.first.c_str())) {
            default: {
                std::cout << Log::err << "Invalid argument " << arg.first << "." << Log::endl;
                return -1;
            }
            case hashStr("--mode"): {
                if      (arg.second == "client") mode = Mode::CLIENT;
                else if (arg.second == "server") mode = Mode::SERVER;
                else if (arg.second == "local" ) mode = Mode::LOCAL_SERVER;
                else std::cout << Log::err << "Invalid mode argument." << Log::endl;
                break;
            }
            case hashStr("--port"): {
                addr.port = stoi(arg.second);
            }
            case hashStr("--address"): {
                addr.host = arg.second;
            }
        }
    }

    //Start the game
    switch (mode) {
        default: {
            std::cout << Log::err << "Mode not setText." << Log::endl;
            return -1;
        }
        case Mode::CLIENT: {
            Client c(argv[0], addr, 1366, 768);
            break;
        }
        case Mode::LOCAL_SERVER: {
            Client c(argv[0], addr, 1366, 768);
            break;
        }
        case Mode::SERVER: {
            Server s(argv[0], addr.port);
            break;
        }
    }

    return 0;
}

#pragma clang diagnostic pop