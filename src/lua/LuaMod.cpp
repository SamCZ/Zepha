//
// Created by aurailus on 05/08/19.
//

#include "LuaMod.h"
#include <gzip/decompress.hpp>

LuaMod LuaMod::fromPacket(const Packet& p) {
    LuaMod luaMod {};

    std::string mod = gzip::decompress(&p.data[4], p.data.size());
    luaMod.serialized = &p.data[4];

    unsigned int offset = 0;

    std::string name = Serializer::decodeString(&mod[0]);
    offset += 4 + name.length();

    std::string description = Serializer::decodeString(&mod[offset]);
    offset += 4 + description.length();

    std::string version = Serializer::decodeString(&mod[offset]);
    offset += 4 + version.length();

    std::string dependsStr = Serializer::decodeString(&mod[offset]);
    offset += 4 + dependsStr.length();

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

    while (offset < mod.length()) {
        std::string path = Serializer::decodeString(&mod[offset]);
        offset += 4 + path.length();
        std::string file = Serializer::decodeString(&mod[offset]);
        offset += 4 + file.length();

        LuaModFile modFile {path, file};
        luaMod.files.push_back(modFile);
    }

    return std::move(luaMod);
}