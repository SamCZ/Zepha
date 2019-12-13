//
// Created by aurailus on 08/01/19.
//

#include "MainMenuScene.h"

MainMenuScene::MainMenuScene(ClientState& state) :
    Scene(state),
    sandbox(sandboxArea, state, sandboxContainer) {

    state.renderer.setClearColor(0, 0, 0);
    state.renderer.window.lockMouse(false);

    Font f(state.defs.textures, state.defs.textures["font"]);
    win = state.renderer.window.getSize();
    sandboxArea = win - glm::ivec2(0, 18 * GS);

    components.add(sandboxContainer);

    branding = std::make_shared<GUIContainer>("zephaBranding");
    components.add(branding);
    {
        auto zephaText = std::make_shared<GUIText>("zephaText");
        zephaText->create({GS, GS}, {}, {}, {1, 1, 1, 1}, f);
        zephaText->setText("Zepha");
        branding->add(zephaText);

        auto alphaText = std::make_shared<GUIText>("alphaText");
        alphaText->create({GS, GS}, {}, {}, {1, 0.5, 0.7, 1}, f);
        alphaText->setText("ALPHA");
        alphaText->setPos({25*GS, 0});
        branding->add(alphaText);
    }

    navigationBar = std::make_shared<GUIContainer>("navigationBar");
    navigationBar->add(std::make_shared<GUIContainer>("navigationBarBg"));
    navigationBar->add(std::make_shared<GUIContainer>("navigationBarIcons"));

    auto navigationBarIcons = navigationBar->get<GUIContainer>("navigationBarIcons");

    components.add(navigationBar);
    {
        auto settingsButton = std::make_shared<GUIImageButton>("settingsButton");
        settingsButton->create({16 * GS, 16 * GS}, {},
                               state.defs.textures["crop(0, 0, 16, 16, menu_flag_settings)"],
                               state.defs.textures["crop(16, 0, 16, 16, menu_flag_settings)"]);
        navigationBar->get<GUIContainer>("navigationBarIcons")->add(settingsButton);

        auto closeButton = std::make_shared<GUIImageButton>("closeButton");
        closeButton->create({16 * GS, 16 * GS}, {},
                            state.defs.textures["crop(0, 0, 16, 16, menu_flag_quit)"],
                            state.defs.textures["crop(16, 0, 16, 16, menu_flag_quit)"]);
        closeButton->setClickCallback([]() { exit(0); });
        navigationBar->get<GUIContainer>("navigationBarIcons")->add(closeButton);

        auto serversButton = std::make_shared<GUIImageButton>("serversButton");
        serversButton->create({16 * GS, 16 * GS}, {},
                              state.defs.textures["crop(0, 0, 16, 16, menu_flag_multiplayer)"],
                              state.defs.textures["crop(16, 0, 16, 16, menu_flag_multiplayer)"]);
        serversButton->setPos({GS, GS});
        navigationBarIcons->add(serversButton);

        auto contentButton = std::make_shared<GUIImageButton>("contentButton");
        contentButton->create({16 * GS, 16 * GS}, {},
                              state.defs.textures["crop(0, 0, 16, 16, menu_flag_content)"],
                              state.defs.textures["crop(16, 0, 16, 16, menu_flag_content)"]);
        contentButton->setPos({GS + GS * 18, GS});
        contentButton->setClickCallback([&](){ state.desiredState = "connect"; });
        navigationBarIcons->add(contentButton);

        auto divider = std::make_shared<GUIRect>("divider");
        divider->create({GS, GS * 10}, {}, {1, 1, 1, 0.3});
        divider->setPos({GS * 2 + GS * 18 * 2, GS * 4});
        navigationBarIcons->add(divider);

        findSubgames();

        for (unsigned int i = 0; i < subgames.size(); i++) {
            auto &subgame = subgames[i];
            auto button = std::make_shared<GUIImageButton>(subgame.config.name);
            button->create({16 * GS, 16 * GS}, {},
                           state.defs.textures["crop(0, 0, 16, 16, " + subgame.iconRef->name + ")"],
                           state.defs.textures["crop(16, 0, 16, 16, " + subgame.iconRef->name + ")"]);
            button->setPos({GS * 7 + GS * 18 * (i + 2), GS});
            button->setClickCallback([&, i]() {
                selectedSubgame = &subgame;
                sandbox.load(*selectedSubgame);
            });
            navigationBarIcons->add(button);
        }
    }

    if (subgames.size() > 0) {
        selectedSubgame = &subgames[0];
        sandbox.load(*selectedSubgame);
    }

    positionElements();

    state.renderer.window.addResizeCallback("mainmenu", [&](glm::ivec2 win) {
        this->win = win;
        sandboxArea = win - glm::ivec2(0, 18 * GS);
        positionElements();
    });
}

void MainMenuScene::findSubgames() {
    std::string subgamesPath = state.path + "subgames";

    cf_dir_t subgamesDir;
    cf_dir_open(&subgamesDir, subgamesPath.data());
    while (subgamesDir.has_next) {
        cf_file_t subgameFolder;
        cf_read_file(&subgamesDir, &subgameFolder);
        if (!subgameFolder.is_dir || strncmp(subgameFolder.name, ".", 1) == 0) { cf_dir_next(&subgamesDir); continue; }

        try {
            bool hasConf = false, hasIcon = false, hasMods = false;

            cf_dir_t subgame;
            cf_dir_open(&subgame, subgameFolder.path);
            while (subgame.has_next) {
                cf_file_t file;
                cf_read_file(&subgame, &file);

                if (!file.is_dir && strncmp(file.name, "icon.png\0", 9) == 0)   hasIcon = true;
                if (!file.is_dir && strncmp(file.name, "conf.json\0", 10) == 0) hasConf = true;
                if ( file.is_dir && strncmp(file.name, "mods\0", 5) == 0)       hasMods = true;

                cf_dir_next(&subgame);
            }
            cf_dir_close(&subgame);

            if (!hasConf)
                throw std::string("Subgame ") + std::string(subgameFolder.name) + " is missing a conf.json.";
            if (!hasMods)
                throw std::string("Subgame ") + std::string(subgameFolder.name) + " is missing a 'mods' directory.";

            json j{};
            try {
                std::ifstream(std::string(subgameFolder.path) + "/conf.json") >> j;
            } catch (...) { throw std::string(subgameFolder.name) + "/conf.json is not a valid JSON object."; }

            if (!j.is_object())
                throw std::string(subgameFolder.name) + "/conf.json is not a valid JSON object.";
            if (!j["name"].is_string() || j["name"] == "")
                throw "The 'name' property in " + std::string(subgameFolder.name) + "/conf.json is missing or invalid.";
            if (!j["version"].is_string() || j["version"] == "")
                throw "The 'version' property in " + std::string(subgameFolder.name) + "/conf.json is missing or invalid.";

            std::string name = j["name"];
            std::string description = (j["description"].is_string() ? j["description"] : "");
            std::string version = j["version"];

            std::shared_ptr<AtlasRef> icon = state.defs.textures["menu_flag_missing"];
            if (hasIcon) icon = state.defs.textures.loadImage(std::string(subgameFolder.path) + "/icon.png", name);

            subgames.push_back({icon, {name, description, version}, subgameFolder.path});
        }
        catch(const std::string& e) {
            std::cout << Log::err << "Encountered an error while loading subgames:\n\t" << e << Log::endl;
        }

        cf_dir_next(&subgamesDir);
    }
    cf_dir_close(&subgamesDir);

    std::sort(subgames.begin(), subgames.end(), [](Subgame& a, Subgame& b) {
        return a.config.name < b.config.name;
    });
}

void MainMenuScene::positionElements() {
    sandbox.windowResized();

    branding->setPos({win.x - 55*GS, win.y - 30*GS});

    navigationBar->setPos({0, win.y - 18*GS});

    auto navigationBarBg = navigationBar->get<GUIContainer>("navigationBarBg");
    for (unsigned int i = 0; i < static_cast<float>(win.x) / 64.f / GS; i++) {
        auto segment = std::make_shared<GUIRect>("segment_" + to_string(i));
        segment->create({64 * GS, 18 * GS}, {}, state.defs.textures["menu_bar_bg"]);
        segment->setPos({i * 64 * GS, 0});
        navigationBarBg->add(segment);
    }

    auto navigationBarIcons = navigationBar->get<GUIContainer>("navigationBarIcons");
    navigationBarIcons->get<GUIImageButton>("closeButton")->setPos({win.x - 16*GS - GS, GS});
    navigationBarIcons->get<GUIImageButton>("settingsButton")->setPos({win.x - 16*GS*2 - GS*3, GS});
}

void MainMenuScene::update() {
    state.defs.textures.update();
    sandbox.update(state.deltaTime);

    state.renderer.window.setCursorHand(components.mouseActivity(state.renderer.window.getMousePos()));

    if (state.renderer.window.input.isMousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
        components.triggerClick(state.renderer.window.getMousePos());
    }
}

void MainMenuScene::draw() {
    state.renderer.beginChunkDeferredCalls();
    state.renderer.endDeferredCalls();

    state.renderer.beginGUIDrawCalls();
    state.renderer.enableTexture(&state.defs.textures.atlasTexture);
    components.draw(state.renderer);
    state.renderer.swapBuffers();
}

void MainMenuScene::cleanup() {
    state.renderer.window.setCursorHand(false);
    state.renderer.window.removeResizeCallback("mainmenu");
}
