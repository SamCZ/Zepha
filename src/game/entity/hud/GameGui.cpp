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
    root->create(bufferSize, {}, {0, 0, 0, 0.25});
    add(root);

    auto inv_root = std::make_shared<GUIRect>("inv_root");
    inv_root->create({648, 270}, {60, 30, 24, 30}, atlas.getTextureRef("inventory"));
    inv_root->setPos({bufferSize.x / 2 - 354, bufferSize.y / 2 - 90});
    root->add(inv_root);

    std::array<std::string, 6> mats = {
        "zeus:materials:stick",
        "zeus:materials:rock",
        "zeus:materials:flint",
        "aurailus:basictools:flint_pickaxe",
        "aurailus:basictools:flint_hatchet",
        "aurailus:basictools:flint_shovel"
    };

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 12; j++) {
            auto inv_slot = std::make_shared<GUIRect>("inv_slot_" + to_string(i) + "_" + to_string(j));
            inv_slot->create({48, 48}, {}, atlas.getTextureRef(mats[rand() % 6]));
            inv_slot->setPos({3 + j * 54, 3 + i * 54});
            inv_root->add(inv_slot);
        }
    }

    auto shortcuts_root = std::make_shared<GUIRect>("shortcut_root");
    shortcuts_root->create({156, 156}, {}, atlas.getTextureRef("inventory_wheel"));
    shortcuts_root->setPos({bufferSize.x / 2 - 72, bufferSize.y - 200});
    root->add(shortcuts_root);

    std::array<glm::vec2, 6> vec_roots = {
        glm::vec2 { 133, -26 },
        glm::vec2 { 155,  37 },
        glm::vec2 { 133,  100},
        glm::vec2 {-241, -26 },
        glm::vec2 {-263,  37 },
        glm::vec2 {-241,  100}
    };

    for (auto i = 0; i < 6; i++) {
        glm::vec2 vec = vec_roots[i];

        auto shortcut_slot = std::make_shared<GUIRect>("shortcut_slot_" + to_string(i));
        shortcut_slot->create({216, 54}, {15, 24, 15, 24}, atlas.getTextureRef("inventory_wheel_slot"));
        shortcut_slot->setPos(vec);
        shortcuts_root->add(shortcut_slot);
    }

    root->setVisible(false);
}

void GameGui::bufferResized(glm::vec2 bufferSize) {
    get<GUIRect>("crosshair")->setPos({bufferSize.x / 2 - 11, bufferSize.y / 2 - 9});
    get<GUIRect>("viginette")->setScale({bufferSize.x, bufferSize.y});
}

void GameGui::setVisible(bool visible) {
    GUIComponent::setVisible(visible);
    get<GUIRect>("root")->setVisible(invOpen);
}

void GameGui::setInvOpen(bool open) {
    get<GUIRect>("root")->setVisible(open);
    invOpen = open;
}

bool GameGui::isInvOpen() {
    return invOpen;
}