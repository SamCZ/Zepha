//
// Created by aurailus on 2019-12-12.
//

#include "MenuSandbox.h"

#include "../../../lua/api/modules/mDelay.h"
#include "../../../lua/api/modules/mPrintE.h"
#include "../../../lua/api/modules/mSetGui.h"
#include "../../../lua/api/modules/mStartGame.h"

MenuSandbox::MenuSandbox(glm::ivec2 &win, ClientState& state, std::shared_ptr<GUIContainer> container) :
        win(win),
        state(state),
        container(container),
        builder(state.defs, container) {}

void MenuSandbox::setup() {
    delayed_functions.clear();
    core = {};
    lua = sol::state {};
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table);

    //Create Zepha Table
    core = lua.create_table();
    lua["zepha"] = core;
    core["__builtin"] = lua.create_table();

    //Sandbox the dofile function
    lua["dofile"] = lua["loadfile"] = sol::nil;
    lua.set_function("runfile", &MenuSandbox::DoFileSandboxed, this);

    MenuApi::delay(core, delayed_functions);
    MenuApi::printe(lua);
    MenuApi::set_gui(builder, win, lua, core);
    MenuApi::start_game(state, core);
}

void MenuSandbox::load(const std::string &subgamePath) {
    container->empty();
    setup();
    loadMod(subgamePath + "/menu");
    DoFileSandboxed("main");
}

void MenuSandbox::windowResized() {
    builder.build(win);
}

sol::protected_function_result MenuSandbox::DoFileSandboxed(std::string file) {
    for (LuaModFile& f : mod.files) {
        if (f.path == file) {

            sol::environment env(lua, sol::create, lua.globals());
            env["_PATH"] = f.path.substr(0, f.path.find_last_of('/') + 1);
            env["_FILE"] = f.path;
            env["_MODNAME"] = mod.config.name;

            auto pfr = lua.safe_script(f.file, env, [&](lua_State*, sol::protected_function_result errPfr) {
                sol::error err = errPfr;
                std::cout << Log::err << file << " returned an error: " << err.what() << Log::endl;
                return errPfr;
            }, "@" + f.path, sol::load_mode::text);

            return pfr;
        }
    }

    std::cout << Log::err << "Error opening \"" + file + "\", not found." << Log::endl;
}

void MenuSandbox::loadMod(const std::string &modPath) {
    mod = LuaMod {};
    std::string root = modPath + "/script";

    std::list<std::string> dirsToScan {root};
    std::list<std::string> luaFiles {};

    cf_dir_t dir;
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

    mod.modPath = modPath;
    auto& conf = mod.config;

//    std::ifstream i(modPath + "/conf.json");
//    json j {};
//    i >> j;
//
//    auto depends = j["depends"];
//    if (strncmp(depends.type_name(), "array", 5) == 0) {
//        for (auto &it : depends) {
//            if (strncmp(it.type_name(), "string", 6) == 0) {
//                conf.depends.push_back(static_cast<std::string>(it));
//            }
//        }
//    }
//
//    conf.name = j["name"];
//    conf.description = j["description"];
//    conf.version = j["version"];

    for (std::string& file : luaFiles) {
        size_t rootPos = file.find(root);
        std::string modPath = file;
        assert(rootPos != std::string::npos);

        modPath.erase(rootPos, root.length() + 1);
//        modPath.insert(0, conf.name);
        modPath.resize(modPath.size() - 4);

        std::ifstream t(file);
        std::string fileStr((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

        LuaModFile f {modPath, fileStr};
        mod.files.push_back(f);
    }
}