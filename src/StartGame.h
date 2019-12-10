//
// Created by aurailus on 22/08/19.
//

#pragma once

#include "game/Client.h"
#include "server/Server.h"
#include "util/Log.h"

enum class Mode { INVALID, CLIENT, SERVER };

uint64_t constexpr mix(char m, uint64_t s) {
    return ((s<<7) + ~(s>>3)) + ~m;
}

uint64_t constexpr hashStr(const char * m) {
    return (*m) ? mix(*m, hashStr(m+1)) : 0;
}

std::map<std::string, std::string> parseArgs(int argc, char* argv[]) {
    //Collect arguments into `args` map
    std::map<std::string, std::string> args;
    for (int i = 1; i < argc; i++) {

        std::string arg(argv[i]);
        size_t equals = arg.find('=');
        std::string first = (equals == -1) ? arg : arg.substr(0, equals);

        if (args.count(first)) {
            std::cerr << "Duplicate argument '" << first << "'" << std::endl;
            exit(1);
        }
        if (equals == -1) args.emplace(first, "");
        else {
            if (equals == arg.length() - 1) {
                std::cerr << "Empty argument '" << first << "'" << std::endl;
                exit(1);
            }
            args.emplace(first, arg.substr(equals + 1, arg.length()));
        }
    }

    return args;
}

int StartGame(int argc, char* argv[]) {
    std::string path = argv[0];

    Address addr {"127.0.0.1", 32000};
    Mode mode = Mode::CLIENT;
    std::string subgame = "";

    //Parse the arguments map
    for (auto arg : parseArgs(argc, argv)) {
        switch (hashStr(arg.first.c_str())) {
            default: {
                std::cout << Log::err << "Invalid argument " << arg.first << "." << Log::endl;
                return -1;
            }
            case hashStr("--mode"): {
                if      (arg.second == "client") mode = Mode::CLIENT;
                else if (arg.second == "server") mode = Mode::SERVER;
                else std::cout << Log::err << "Invalid mode argument." << Log::endl;
                break;
            }
            case hashStr("--port"): {
                addr.port = static_cast<unsigned short>(stoi(arg.second));
            }
            case hashStr("--address"): {
                addr.host = arg.second;
            }
            case hashStr("--subgame"): {
                subgame = arg.second;
            }
        }
    }

    // Obligatory ASCII Art is obligatory.
    Log::clear();
    std::cout << "\n"
                 "\t\t▒███████▒▓█████  ██▓███   ██░ ██  ▄▄▄      \n"
                 "\t\t▒ ▒ ▒ ▄▀░▓█   ▀ ▓██░  ██▒▓██░ ██▒▒████▄    \n"
                 "\t\t░ ▒ ▄▀▒░ ▒███   ▓██░ ██▓▒▒██▀▀██░▒██  ▀█▄  \n"
                 "\t\t  ▄▀▒   ░▒▓█  ▄ ▒██▄█▓▒ ▒░▓█ ░██ ░██▄▄▄▄██ \n"
                 "\t\t▒███████▒░▒████▒▒██▒ ░  ░░▓█▒░██▓ ▓█   ▓██▒\n"
                 "\t\t░▒▒ ▓░▒░▒░░ ▒░ ░▒▓▒░ ░  ░ ▒ ░░▒░▒ ▒▒   ▓▒█░\n"
                 "\t\t░ ▒ ▒ ░ ▒ ░ ░  ░░▒ ░      ▒ ░▒░ ░  ▒   ▒▒ ░\n"
                 "\t\t  ░ ░   ░   ░   ░░        ░  ░     ░   ▒   \n" << std::endl;

    //Start the game
    switch (mode) {
        default: {
            std::cout << Log::err << "Mode not set." << Log::endl;
            return -1;
        }
        case Mode::CLIENT: {
            Client c(addr, {1366, 768});
            break;
        }
//        case Mode::LOCAL_SERVER: {
//            auto localServer = std::make_unique<LocalServerInstance>(path, addr.port);
//            Client c(std::move(localServer), {1366, 768});
//            break;
//        }
        case Mode::SERVER: {
            Server s(path, addr.port, subgame);
            break;
        }
    }

    return 0;
};

