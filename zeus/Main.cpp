#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define STB_IMAGE_IMPLEMENTATION
#pragma clang diagnostic pop

#include <zconf.h>
#include "client/Client.h"

int main(int argc, char* argv[]) {
    std::string start = "client";
    if (argc >= 2) {
        start = argv[1];
    }

    if (start == "client") {
        Client c(1366, 768);
        c.start(argv[0]); //TODO: Get this to client some other way
    } else if (start == "server") {
        std::cout << "ZEUS SERVER" << std::endl;
        Server s(12345);
        s.start();
    }

    return 0;
}