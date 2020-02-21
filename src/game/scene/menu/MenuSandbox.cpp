//
// Created by aurailus on 2019-12-12.
//

#include "MenuSandbox.h"

#include "../../../lua/api/menu/mDelay.h"
#include "../../../lua/api/menu/mSetGui.h"
#include "../../../lua/api/menu/mStartGame.h"

MenuSandbox::MenuSandbox(glm::ivec2 &win, ClientState& state, std::shared_ptr<GuiContainer> container) :
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

    //Sandbox the dofile function
    lua["dofile"] = lua["loadfile"] = sol::nil;
    lua.set_function("runfile", &MenuSandbox::DoFileSandboxed, this);

    MenuApi::delay(core, delayed_functions);
    MenuApi::set_gui(builder, win, lua, core);
    MenuApi::start_game(state, core);
}

void MenuSandbox::load(const Subgame& subgame) {
    builder.clear();
    setup();

    try {
        loadMod(subgame.subgamePath + "/menu");
        DoFileSandboxed("init");
    }
    catch (const std::string& e) {
        std::cout << Log::err <<
            "Encountered an error loading menu mod for subgame '" + subgame.config.name + "':\n\t"
            << e << Log::endl;
    }
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

    throw std::string("Error executing file '" + file + "', file not found.");
}

void MenuSandbox::loadMod(const std::string &modPath) {
    if (!cf_file_exists(modPath.data())) throw std::string("Directory not found.");

    mod = LuaMod {};
    std::string root = modPath + "/script";

    std::list<std::string> dirsToScan {root};
    std::list<std::string> luaFiles {};

    cf_dir_t dir;
    while (!dirsToScan.empty()) {
        std::string dirStr = *dirsToScan.begin();
        dirsToScan.erase(dirsToScan.begin());

        if (!cf_file_exists(dirStr.data())) throw std::string("Missing 'script' directory.");
        cf_dir_open(&dir, dirStr.data());

        while (dir.has_next) {
            // Read through files in the directory
            cf_file_t scannedFile;
            cf_read_file(&dir, &scannedFile);

            if (strncmp(scannedFile.name, ".", 1) != 0) {
                if (scannedFile.is_dir) dirsToScan.emplace_back(scannedFile.path);
                else {
                    char *dot = strrchr(scannedFile.path, '.');
                    if (dot && strlen(dot) >= 4 && strncmp(dot, ".lua", 4) == 0) {
                        luaFiles.emplace_back(scannedFile.path);
                    }
                }
            }
            cf_dir_next(&dir);
        }
        cf_dir_close(&dir);
    }

    mod.modPath = modPath;

    for (std::string& file : luaFiles) {
        size_t rootPos = file.find(root);
        std::string modPath = file;

        if (rootPos == std::string::npos)
            throw std::string("Attempted to access file '") + file + "' which is outside of mod root '" + root + "'.";

        modPath.erase(rootPos, root.length() + 1);
        modPath.resize(modPath.size() - 4);

        std::ifstream t(file);
        std::string fileStr((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

        LuaModFile f {modPath, fileStr};
        mod.files.push_back(f);
    }

    std::string texPath = modPath + "/textures";
    if (cf_file_exists(texPath.data())) {
        this->modAssets = state.defs.textures.loadDirectory(texPath, false, true);
    }
}