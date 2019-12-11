//
// Created by aurailus on 08/01/19.
//

#include "MainMenuScene.h"
#include "../hud/components/compound/GUIImageButton.h"

MainMenuScene::MainMenuScene(ClientState& state) : Scene(state) {
    state.renderer.setClearColor(22, 22, 22);
    state.renderer.getWindow().lockMouse(false);

    Font f(state.defs.textures, state.defs.textures["font"]);
    glm::vec2 size = state.renderer.getWindow().getSize();

    subgame = std::make_shared<GUIRect>("subgameContainer");
    subgame->create(size, {}, state.defs.textures["zeus_background"]);
    components.add(subgame);

    auto menubar = std::make_shared<GUIRect>("menubar");
    menubar->create({90*GS, size.y}, {}, {0.1, 0.15, 0.2, 0.3}, {0.1, 0.15, 0.2, 0.3}, {0.1, 0.15, 0.2, 0.7}, {0.1, 0.15, 0.2, 0.7});
    subgame->add(menubar);

    auto zeusLogo = std::make_shared<GUIRect>("zeusLogo");
    zeusLogo->create({GS*86, GS*30}, {}, state.defs.textures["zeus_logo"]);
    zeusLogo->setPos({2*GS, 3*GS});
    menubar->add(zeusLogo);

    auto zeusPlayButton = std::make_shared<GUIImageButton>("zeusPlayButton");
    zeusPlayButton->create({GS*82, GS*13}, {}, state.defs.textures["crop(0, 0, 82, 13, zeus_button)"],
            state.defs.textures["crop(0, 13, 82, 13, zeus_button)"]);
    zeusPlayButton->setPos({3*GS, 48*GS});
    zeusPlayButton->setClickCallback([&]() {
        state.desiredState = "connect"; //TODO: stop with this gross string thing
    });
    menubar->add(zeusPlayButton);

    auto zeusPlayButtonText = std::make_shared<GUIText>("text");
    zeusPlayButtonText->create({GS, GS}, {}, {}, {1, 0.95, 0.5, 1}, f);
    zeusPlayButtonText->setText("Play~");
    zeusPlayButtonText->setPos({31*GS,2*GS});
    zeusPlayButton->add(zeusPlayButtonText);

    auto zeusServersButton = std::make_shared<GUIImageButton>("zeusServersButton");
    zeusServersButton->create({GS*82, GS*13}, {}, state.defs.textures["crop(0, 0, 82, 13, zeus_button)"],
                              state.defs.textures["crop(0, 13, 82, 13, zeus_button)"]);
    zeusServersButton->setPos({3*GS, 64*GS});
    zeusServersButton->setClickCallback([&]() {
        if (showingSubgame) {
            components.remove(subgame->getKey());
            showingSubgame = false;
        }
    });
    menubar->add(zeusServersButton);

    auto zeusServersButtonText = std::make_shared<GUIText>("text");
    zeusServersButtonText->create({GS, GS}, {}, {}, {1, 0.95, 0.5, 1}, f);
    zeusServersButtonText->setText("Servers");
    zeusServersButtonText->setPos({26*GS,2*GS});
    zeusServersButton->add(zeusServersButtonText);

    auto zephaText = std::make_shared<GUIText>("zephaText");
    zephaText->create({GS*2, GS*2}, {}, {}, {1,1,1,1}, f);
    zephaText->setText("Zepha");
    zephaText->setPos({size.x - 48*GS, size.y - 44*GS});
    components.add(zephaText);

    auto alphaText = std::make_shared<GUIText>("alphaText");
    alphaText->create({GS, GS}, {}, {}, {1,0.5,0.7,1}, f);
    alphaText->setText("ALPHA");
    alphaText->setPos({size.x - 27*GS, size.y - 27*GS});
    components.add(alphaText);

    auto bottomBar = std::make_shared<GUIContainer>("bottomBar");
    bottomBar->setPos({0, size.y - 18*GS});
    components.add(bottomBar);

    auto bottomBarBg = std::make_shared<GUIContainer>("bottomBarBg");
    bottomBar->add(bottomBarBg);

    auto bottomBarElems = std::make_shared<GUIContainer>("bottomBarElems");
    bottomBar->add(bottomBarElems);

    // Subgame Buttons

    auto zeusButton = std::make_shared<GUIImageButton>("zeusButton");
    zeusButton->create({16*GS, 16*GS}, {}, state.defs.textures["crop(0, 0, 16, 16, menu_flag_zeus)"],
            state.defs.textures["crop(16, 0, 16, 16, menu_flag_zeus)"]);
    zeusButton->setPos({GS, GS});
    zeusButton->setClickCallback([&](){
        if (!showingSubgame) {
            components.insert(0, subgame);
            showingSubgame = true;
        }
    });
    bottomBarElems->add(zeusButton);

    auto serversButton = std::make_shared<GUIImageButton>("serversButton");
    serversButton->create({16*GS, 16*GS}, {}, state.defs.textures["crop(0, 0, 16, 16, menu_flag_multiplayer)"],
            state.defs.textures["crop(16, 0, 16, 16, menu_flag_multiplayer)"]);
    serversButton->setPos({16*GS + GS*3, GS});
    serversButton->setClickCallback([&](){
        if (showingSubgame) {
            components.remove(subgame->getKey());
            showingSubgame = false;
        }
    });
    bottomBarElems->add(serversButton);

    auto contentButton = std::make_shared<GUIImageButton>("contentButton");
    contentButton->create({16*GS, 16*GS}, {}, state.defs.textures["crop(0, 0, 16, 16, menu_flag_content)"],
            state.defs.textures["crop(16, 0, 16, 16, menu_flag_content)"]);
    contentButton->setPos({16*GS*2 + GS*5, GS});
    contentButton->setClickCallback([&](){
        if (showingSubgame) {
            components.remove(subgame->getKey());
            showingSubgame = false;
        }
    });
    bottomBarElems->add(contentButton);

    // Meta buttons

    auto settingsButton = std::make_shared<GUIImageButton>("settingsButton");
    settingsButton->create({16*GS, 16*GS}, {}, state.defs.textures["crop(0, 0, 16, 16, menu_flag_settings)"],
            state.defs.textures["crop(16, 0, 16, 16, menu_flag_settings)"]);
    settingsButton->setPos({size.x - 16*GS*2 - GS*3, GS});
    settingsButton->setClickCallback([](){
        std::cout << "Settings" << std::endl;
    });
    bottomBarElems->add(settingsButton);

    auto closeButton = std::make_shared<GUIImageButton>("closeButton");
    closeButton->create({16*GS, 16*GS}, {}, state.defs.textures["crop(0, 0, 16, 16, menu_flag_quit)"],
            state.defs.textures["crop(16, 0, 16, 16, menu_flag_quit)"]);
    closeButton->setPos({size.x - 16*GS - GS, GS});
    closeButton->setClickCallback([](){ exit(0); });
    bottomBarElems->add(closeButton);

    for (unsigned int i = 0; i < size.x/64/GS; i++) {
        auto bottomBarSegment = std::make_shared<GUIRect>("bottomBarSegment" + to_string(i));
        bottomBarSegment->create({64*GS, 18*GS}, {}, state.defs.textures["menu_bar_bg"]);
        bottomBarSegment->setPos({i*64*GS, 0});
        bottomBarBg->add(bottomBarSegment);
    }
}

void MainMenuScene::update() {
    if (state.renderer.resized) {
        auto size = state.renderer.getWindow().getSize();

        subgame->setScale(size);
        subgame->get<GUIRect>("menubar")->setScale({90*GS, size.y});

        auto zeusLogo = subgame->get<GUIRect>("menubar")->get<GUIRect>("zeusLogo");
        zeusLogo->setPos({2*GS, 3*GS});
        zeusLogo->setScale({GS*86, GS*30});

        components.get<GUIText>("zephaText")->setPos({size.x - 48*GS, size.y - 44*GS});
        components.get<GUIText>("alphaText")->setPos({size.x - 27*GS, size.y - 27*GS});

        auto bottomBar = components.get<GUIContainer>("bottomBar");
        bottomBar->setPos({0, size.y - 18*GS});
        auto bottomBarElems = bottomBar->get<GUIContainer>("bottomBarElems");

        auto settingsButton = bottomBarElems->get<GUIRect>("settingsButton");
        settingsButton->setPos({size.x - 16*GS*2 - GS*3, GS});

        auto closeButton = bottomBarElems->get<GUIRect>("closeButton");
        closeButton->setPos({size.x - 16*GS - GS, GS});

        auto bottomBarBg = bottomBar->get<GUIContainer>("bottomBarBg");
        bottomBarBg->empty();
        for (unsigned int i = 0; i < size.x/64/GS; i++) {
            auto bottomBarSegment = std::make_shared<GUIRect>("bottomBarSegment" + to_string(i));
            bottomBarSegment->create({64*GS, 18*GS}, {}, state.defs.textures["menu_bar_bg"]);
            bottomBarSegment->setPos({i*64*GS, 0});
            bottomBarBg->add(bottomBarSegment);
        }

        state.renderer.resized = false;
    }

    components.mouseActivity(state.renderer.getWindow().getMousePos());
    if (state.renderer.getWindow().input.isMousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
        components.triggerClick(state.renderer.getWindow().getMousePos());
    }
}

void MainMenuScene::draw() {
    Renderer& renderer = state.renderer;

    renderer.beginChunkDeferredCalls();
    renderer.endDeferredCalls();
    renderer.beginGUIDrawCalls();
    renderer.enableTexture(&state.defs.textures.atlasTexture);

    components.draw(renderer);

    renderer.swapBuffers();
}