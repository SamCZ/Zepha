//
// Created by aurailus on 2019-12-12.
//

#include "MenuSandbox.h"
#include "../../../lua/ErrorFormatter.h"

#include "../../../lua/api/menu/mDelay.h"
#include "../../../lua/api/menu/mSetGui.h"
#include "../../../lua/api/menu/mStartGame.h"

#include "../../../lua/api/class/LuaGuiElement.h"

MenuSandbox::MenuSandbox(glm::ivec2 &win, ClientState& state, std::shared_ptr<GuiContainer> container) :
    win(win),
    state(state),
    container(container),
    builder(state.defs.textures, state.defs.models, container) {}

void MenuSandbox::reset() {
    builder.clear(true);
    delayed_functions.clear();
    core = {};
    mod = {};
    lua = sol::state {};
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table);

    loadApi();
}

void MenuSandbox::loadApi() {
    //Create Zepha Table
    core = lua.create_table();
    lua["zepha"] = core;
    core["__builtin"] = lua.create_table();

    ClientApi::gui_element(lua);

    MenuApi::delay      (core, delayed_functions);
    MenuApi::set_gui    (builder, win, lua, core);
    MenuApi::start_game (state, core);

    // Create sandboxed runfile()
    lua["dofile"] = lua["loadfile"] = sol::nil;
    lua.set_function("runfile", &MenuSandbox::runFileSandboxed, this);
}

void MenuSandbox::load(const Subgame& subgame) {
    reset();

    try {
        loadAndRunMod(subgame.subgamePath + "/../../assets/base");
        loadAndRunMod(subgame.subgamePath + "/menu");
    }
    catch (const std::string& e) {
        std::cout << Log::err << "Encountered an error loading menu mod for subgame '" + subgame.config.name + "':\n\t" << e << Log::endl;
    }
}

void MenuSandbox::windowResized() {
    builder.build(win);
}

void MenuSandbox::update(double delta) {
    LuaParser::update(delta);
    builder.update();
}

sol::protected_function_result MenuSandbox::runFileSandboxed(const std::string& file) {
    for (LuaModFile& f : mod.files) {
        if (f.path == file) {
            sol::environment env(lua, sol::create, lua.globals());
            env["_PATH"] = f.path.substr(0, f.path.find_last_of('/') + 1);
            env["_FILE"] = f.path;
            env["_MODNAME"] = mod.config.name;

            return lua.safe_script(f.file, env, std::bind(&MenuSandbox::errorCallback, this,
                    std::placeholders::_2), "@" + f.path, sol::load_mode::text);
        }
    }
}

void MenuSandbox::loadAndRunMod(const std::string &modPath) {
    if (!cf_file_exists(modPath.data())) throw std::string("Directory not found.");

    LuaMod mod;
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

    this->mod = mod;
    runFileSandboxed("main");
}

sol::protected_function_result MenuSandbox::errorCallback(sol::protected_function_result errPfr) {
    sol::error err = errPfr;
    std::string errString = err.what();

    try {
        std::string::size_type lineNumStart = errString.find(':');
        assert(lineNumStart != std::string::npos);
        std::string::size_type lineNumEnd = errString.find(':', lineNumStart + 1);
        assert(lineNumEnd != std::string::npos);

        std::string fileName = errString.substr(0, lineNumStart);
        int lineNum = std::stoi(errString.substr(lineNumStart + 1, lineNumEnd - lineNumStart - 1));

        for (LuaModFile &f : mod.files) {
            if (f.path == fileName) {
                std::cout << std::endl << ErrorFormatter::formatError(fileName, lineNum, errString, f.file) << std::endl;
                exit(1);
            }
        }
    }
    catch (...) {
        std::cout << std::endl << Log::err << errString << Log::endl;
    }
    exit(1);
}