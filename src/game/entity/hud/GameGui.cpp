//
// Created by aurailus on 05/02/19.
//

#include "GameGui.h"

GameGui::GameGui(glm::vec2 bufferSize, TextureAtlas& atlas) {
    auto crosshair = std::make_shared<GUIRect>("crosshair");
    crosshair->create({22, 22}, {}, atlas.getTextureRef("crosshair"));
    crosshair->setPos({bufferSize.x / 2 - 11, bufferSize.y / 2 - 9});
    add(crosshair);

    auto viginette = std::make_shared<GUIRect>("viginette");
    viginette->create(bufferSize, {}, atlas.getTextureRef("viginette"));
    add(viginette);

    auto root = std::make_shared<GUIRect>("root");
    root->create(bufferSize, {}, {0, 0, 0, 0.15});
    add(root);

    auto inv_root = std::make_shared<GUIRect>("inv_root");
    inv_root->create({576, 252}, {60, 30, 12, 30}, atlas.getTextureRef("inventory"));
    inv_root->setPos({bufferSize.x / 2 - 312, bufferSize.y / 2 - 132});
    root->add(inv_root);

    std::array<std::string, 6> mats = {
            "materials_stick",
            "materials_rock",
            "materials_flint",
            "basictools_flint_pickaxe",
            "basictools_flint_hatchet",
            "basictools_flint_shovel"
    };

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 12; j++) {
            auto inv_slot = std::make_shared<GUIRect>("inv_slot" + to_string(i) + to_string(j));
            inv_slot->create({48, 48}, {}, atlas.getTextureRef(mats[rand() % 6]));
            inv_slot->setPos({j * 48, i * 48});
            inv_root->add(inv_slot);
        }
    }

    auto shortcuts_root = std::make_shared<GUIRect>("shortcut_root");
    shortcuts_root->create({156, 156}, {}, atlas.getTextureRef("inventory_wheel"));
    shortcuts_root->setPos({bufferSize.x / 2 - 72, bufferSize.y / 2 + 204});
    root->add(shortcuts_root);

    std::array<glm::vec2, 6> vec_roots = {
        glm::vec2 {133, -11},
        glm::vec2 {149, 46},
        glm::vec2 {133, 103},
        glm::vec2 {-216, -11},
        glm::vec2 {-232, 46},
        glm::vec2 {-216, 103}
    };

    for (auto i = 0; i < 6; i++) {
        glm::vec2 vec = vec_roots[i];
//        for (auto j = 0; j < 4; j++) {
//            auto shortcut_slot = std::make_shared<GUIRect>("shortcut_slot" + to_string(i) + to_string(j));
//            shortcut_slot->create({48, 48}, {}, atlas.getTextureRef("inventory_slot"));
//            shortcut_slot->setPos(vec + glm::vec2 {j * 48, 0});
//            shortcuts_root->add(shortcut_slot);
//        }

        auto shortcut_slot = std::make_shared<GUIRect>("shortcut_slot" + to_string(i));
        shortcut_slot->create({240, 72}, {}, atlas.getTextureRef("inventory_wheel_slot"));
        shortcut_slot->setPos(vec);
        shortcuts_root->add(shortcut_slot);
    }
}

void GameGui::bufferResized(glm::vec2 bufferSize) {
    get<GUIRect>("crosshair")->setPos({bufferSize.x / 2 - 11, bufferSize.y / 2 - 9});
    get<GUIRect>("viginette")->setScale({bufferSize.x, bufferSize.y});
}
