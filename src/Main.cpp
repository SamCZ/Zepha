#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define STB_IMAGE_IMPLEMENTATION
#define CUTE_FILES_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#pragma clang diagnostic pop

#include "game/Game.h"
#include "server/Server.h"

int main(int argc, char* argv[]) {
    std::string arg = "client";
    if (argc >= 2) arg = argv[1];

    if (arg == "client" || arg == "local") {
        Game g(1366, 768, (arg == "local") ? argv[0] : nullptr);
    }

    if (arg == "server" || arg == "local") {
        Server s(12345);
    }

    return 0;
}