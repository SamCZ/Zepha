//
// Created by aurailus on 11/06/19.
//

#include <iomanip>
#include <gzip/compress.hpp>
#include "ServerLuaParser.h"
#include "ServerRegisterBlocks.h"
#include "../../def/ServerDefs.h"

#include "../modules/sDump.h"
#include "../modules/sPrintE.h"

#include "../modules/sIsServer.h"
#include "../modules/sDelay.h"

#include "../modules/sRegisterBlock.h"
#include "../modules/sRegisterBlockmodel.h"

#include "../modules/sSetBlock.h"
#include "../modules/sGetBlock.h"
#include "../modules/sRemoveBlock.h"

void ServerLuaParser::init(ServerDefs& defs, ServerWorld& world, std::string path) {
    //Load Base Libraries
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math);

    //Define Panic Callback
    lua_atpanic(lua, sol::c_call<decltype(&LuaParser::override_panic), &LuaParser::override_panic>);

    //Load Modules
    loadModules(defs, world);

    //Load Mods
    loadMods(defs, path + "mods");

    //Register Blocks
    registerBlocks(defs);
}

void ServerLuaParser::loadModules(ServerDefs &defs, ServerWorld &world) {
    //Create Zeus Table
    zeus = lua.create_table();
    lua["zeus"] = zeus;

    //Load Modules
    ServerApi::dump(lua);
    ServerApi::printe(lua);

    ServerApi::is_server(zeus);
    ServerApi::delay(zeus, delayed_functions);

    ServerApi::register_block(lua, zeus);
    ServerApi::register_blockmodel(lua, zeus);

    ServerApi::get_block(zeus, defs, world);
    ServerApi::set_block(zeus, defs, world);
    ServerApi::remove_block(zeus, defs, world);

    //Sandbox the dofile function
    lua.set_function("dofile", &ServerLuaParser::DoFileSandboxed, this);
}

void ServerLuaParser::registerBlocks(ServerDefs& defs) {
    ServerRegisterBlocks(zeus, defs);
}

void ServerLuaParser::loadMods(ServerDefs& defs, const std::string& rootPath) {
    auto modDirs = findModDirs(rootPath);
    mods = createLuaMods(modDirs);
    createTextures(defs);
    handleDependencies();
    serializeMods();

    for (LuaMod& mod : mods)
        DoFileSandboxed(mod.config.name + "/main");
}

std::list<std::string> ServerLuaParser::findModDirs(const std::string& rootPath) {
    //Find Mod Directories
    std::list<std::string> modDirs {};
    std::list<std::string> dirsToScan {rootPath};

    cf_dir_t dir;

    while (!dirsToScan.empty()) {
        std::string dirStr = *dirsToScan.begin();
        dirsToScan.erase(dirsToScan.begin());
        bool isModFolder = false;

        cf_dir_open(&dir, dirStr.c_str());

        std::list<std::string> subDirs;

        while (dir.has_next) {
            // Read through files in the directory
            cf_file_t scannedFile;
            cf_read_file(&dir, &scannedFile);

            if (strncmp(scannedFile.name, ".", 1) != 0) {
                if (scannedFile.is_dir) subDirs.emplace_back(scannedFile.path);
                else if (strncmp(scannedFile.name, "conf.json", 10) == 0) {
                    isModFolder = true;
                    break;
                }
            }

            cf_dir_next(&dir);
        }

        cf_dir_close(&dir);

        if (isModFolder) modDirs.push_back(dirStr);
        else for (const std::string& s : subDirs) dirsToScan.push_back(s);
    }

    return std::move(modDirs);
}

std::vector<LuaMod> ServerLuaParser::createLuaMods(std::list<std::string> modDirs) {
    cf_dir_t dir;

    std::vector<LuaMod> mods;

    for (const std::string& modDir : modDirs) {
        std::string root = modDir + "/script";

        std::list<std::string> dirsToScan {root};
        std::list<std::string> luaFiles {};

        while (!dirsToScan.empty()) {
            std::string dirStr = *dirsToScan.begin();
            dirsToScan.erase(dirsToScan.begin());

            cf_dir_open(&dir, dirStr.c_str());

            while (dir.has_next) {
                // Read through files in the directory
                cf_file_t scannedFile;
                cf_read_file(&dir, &scannedFile);

                if (strncmp(scannedFile.name, ".", 1) != 0) {
                    if (scannedFile.is_dir) dirsToScan.emplace_back(scannedFile.path);
                    else {
                        char *dot = strrchr(scannedFile.path, '.');
                        if (dot && strncmp(dot, ".lua", 4) == 0) {
                            luaFiles.emplace_back(scannedFile.path);
                        }
                    }
                }

                cf_dir_next(&dir);
            }

            cf_dir_close(&dir);
        }

        LuaMod mod {};
        mod.modPath = modDir;
        auto& conf = mod.config;

        std::ifstream i(modDir + "/conf.json");
        json j;
        i >> j;

        auto depends = j["depends"];
        if (strncmp(depends.type_name(), "array", 5) == 0) {
            for (auto &it : depends) {
                if (strncmp(it.type_name(), "string", 6) == 0) {
                    conf.depends.push_back(static_cast<std::string>(it));
                }
            }
        }

        conf.name = j["name"];
        conf.description = j["description"];
        conf.version = j["version"];

        for (std::string& file : luaFiles) {
            size_t rootPos = file.find(root);
            std::string modPath = file;
            assert(rootPos != std::string::npos);

            modPath.erase(rootPos, root.length());
            modPath.insert(0, conf.name);
            modPath.resize(modPath.size() - 4);

            std::ifstream t(file);
            std::string fileStr((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
            fileStr.insert(0, "local PATH = \"" + modPath + "/\"; ");

            LuaModFile f {modPath, fileStr};
            mod.files.push_back(f);
        }

        mods.push_back(mod);
    }

    return mods;
}

void ServerLuaParser::createTextures(ServerDefs &defs) {
    std::vector<std::string> seekDirs;
    std::vector<std::string> texturePaths;

    cf_dir_t dir;

    for (const LuaMod& mod : mods) {
        std::string root = mod.modPath + "/textures";

        std::list<std::string> dirsToScan {root};

        while (!dirsToScan.empty()) {
            std::string dirStr = *dirsToScan.begin();
            dirsToScan.erase(dirsToScan.begin());

            cf_dir_open(&dir, dirStr.c_str());

            while (dir.has_next) {
                // Read through files in the directory
                cf_file_t scannedFile;
                cf_read_file(&dir, &scannedFile);

                if (strncmp(scannedFile.name, ".", 1) != 0) {
                    if (scannedFile.is_dir) dirsToScan.emplace_back(scannedFile.path);
                    else {
                        char *dot = strrchr(scannedFile.path, '.');
                        if (dot && strncmp(dot, ".png", 4) == 0) {

                            std::string name = std::string(scannedFile.name).substr(0, std::string(scannedFile.name).size() - 4);
                            name.insert(0, mod.config.name + ":");

                            int width, height;
                            unsigned char* data = stbi_load(scannedFile.path, &width, &height, nullptr, 4);
                            std::string str(reinterpret_cast<char*>(data), static_cast<unsigned long>(width * height * 4));
                            std::string comp = gzip::compress(str.data(), str.length());
                            free(data);

                            defs.textures().textures.push_back({std::move(name), comp, width, height});
                        }
                    }
                }

                cf_dir_next(&dir);
            }

            cf_dir_close(&dir);
        }
    }


}

void ServerLuaParser::handleDependencies() {
    for (int i = 0; i < mods.size(); i++) {
        LuaMod& mod = mods[i];
        auto& deps = mod.config.depends;

        bool modifiedList = false;

        for (std::string& dep : deps) {
            for (int j = 0; j < mods.size(); j++) {
                LuaMod& otherMod = mods[j];
                if (otherMod.config.name == dep) {
                    if (j > i) {
                        LuaMod copy = otherMod;
                        mods.erase(mods.begin() + j);
                        mods.insert(mods.begin() + i, copy);
                        i++;
                        modifiedList = true;
                        break;
                    }
                }
            }
        }

        if (modifiedList) i = -1;
    }
}

void ServerLuaParser::serializeMods() {
    for (LuaMod& mod : mods) {
        std::string s;

        Serializer::encodeString(s, mod.config.name);
        Serializer::encodeString(s, mod.config.description);
        Serializer::encodeString(s, mod.config.version);

        std::string depends;
        bool delimiter = false;
        for (const std::string& dep : mod.config.depends) {
            if (delimiter) depends.append(",");
            else delimiter = true;
            depends.append(dep);
        }

        Serializer::encodeString(s, depends);

        for (LuaModFile& file : mod.files) {
            Serializer::encodeString(s, file.path);
            Serializer::encodeString(s, file.file);
        }

        std::string comp = gzip::compress(s.data(), s.length());
        mod.serialized = comp;
    }
}


int ServerLuaParser::DoFileSandboxed(std::string file) {
    size_t modname_length = file.find('/');
    std::string modname = file.substr(0, modname_length);

    for (LuaMod& mod : mods) {
        if (strncmp(mod.config.name.c_str(), modname.c_str(), modname_length) == 0) {
            for (LuaModFile& f : mod.files) {
                if (f.path == file) {
                    lua.script(f.file);
                    return 0;
                }
            }
            break;
        }
    }

    std::cout << Log::err << "Error opening \"" + file + "\", not found." << Log::endl;
}
