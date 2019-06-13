#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define STB_IMAGE_IMPLEMENTATION
#define CUTE_FILES_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#pragma clang diagnostic pop

#include "game/Game.h"
#include "server/Server.h"
#include "util/Log.h"

int main(int argc, char* argv[]) {
    std::cout << Log::info << "Zeus Engine Version 0.0.1 Pre-Alpha" << Log::endl;

    std::string arg = "client";
    if (argc >= 2) arg = argv[1];

    if (arg == "client" || arg == "local") {
        Game g(1366, 768, (arg == "local") ? argv[0] : nullptr);
    }

    if (arg == "server") {
        Server s(12345);
    }

    return 0;
}