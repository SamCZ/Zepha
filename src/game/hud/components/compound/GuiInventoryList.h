//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include "../basic/GuiRect.h"
#include "../basic/GuiContainer.h"

class LocalSubgame;
class LuaGuiElement;
class LocalInventoryRefs;
class LocalInventoryList;

class GuiInventoryList : public GuiContainer {
public:
    GuiInventoryList() = default;
    GuiInventoryList(const std::string& key);
    ~GuiInventoryList() override;

    static std::shared_ptr<GuiInventoryList> fromSerialized(const LuaGuiElement& elem, LocalSubgame &game,
            glm::ivec2 bounds, LocalInventoryRefs& refs);

    void create(glm::vec2 scale, glm::vec4 padding, glm::ivec2 innerPadding,
                std::shared_ptr<LocalInventoryList> list, std::shared_ptr<LocalInventoryList> cursor, LocalSubgame& defs,
                unsigned short start = 0, unsigned short length = 0);

    void setCallback(CallbackType type, const callback& cb) override;

    void hoverEvent(bool hovered, glm::ivec2 pos);
    void leftClick(bool down, glm::ivec2 pos);
    void rightClick(bool down, glm::ivec2 pos);
    void drawContents();
private:
    std::shared_ptr<GuiRect> hoverRect = std::make_shared<GuiRect>("hover_rect");
    std::shared_ptr<std::function<void()>> myCallback = nullptr;

    std::shared_ptr<LocalInventoryList> list, cursor;
    unsigned short start, length;
    glm::ivec2 innerPadding;
    LocalSubgame* defs = nullptr;
};
