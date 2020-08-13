//
// Created by aurailus on 05/08/19.
//

#include <gzip/decompress.hpp>

#include "LuaMod.h"

#include "util/net/PacketView.h"

LuaMod LuaMod::fromPacket(PacketView& p) {
    LuaMod luaMod {};

    auto serialized = p.d.read<std::string>();
    std::string mod = gzip::decompress(serialized.c_str(), serialized.length());
    luaMod.serialized = serialized;

    std::string name, description, version, dependsStr;
    Deserializer d(mod);
    d.read(name).read(description).read(version).read(dependsStr);

    std::vector<std::string> depends;
    size_t pos = 0;
    std::string token;
    while ((pos = dependsStr.find(',')) != std::string::npos) {
        token = dependsStr.substr(0, pos);
        depends.push_back(token);
        dependsStr.erase(0, pos + 1);
    }
    depends.push_back(dependsStr);

    luaMod.config = {name, description, version, depends};

    while (!d.atEnd()) luaMod.files.push_back({ d.read<std::string>(), d.read<std::string>() });

    return std::move(luaMod);
}