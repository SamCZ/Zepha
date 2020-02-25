//
// Created by aurailus on 05/02/19.
//

#include "GameGui.h"
#include "components/compound/GuiInventoryList.h"

GameGui::GameGui(LocalInventoryRefs& refs, glm::vec2 bufferSize, ClientGame& defs, Renderer& renderer) :
    menuRoot(std::make_shared<GuiContainer>("__luaroot")),
    handList(std::make_shared<GuiInventoryList>("hand")),
    builder(refs, defs, menuRoot),
    renderer(renderer),
    win(bufferSize),
    refs(refs),
    defs(defs) {

    auto crosshair = std::make_shared<GuiRect>("crosshair");
    crosshair->create({22, 22}, {}, defs.textures["crosshair"]);
    crosshair->setPos({bufferSize.x / 2 - 11, bufferSize.y / 2 - 9});
    builtIn.add(crosshair);

    auto viginette = std::make_shared<GuiRect>("viginette");
    viginette->create(bufferSize, {}, defs.textures["viginette"]);
    builtIn.add(viginette);

    add(menuRoot);

    handList->create({3, 3}, {}, {}, refs.getHand(), refs.getHand(), defs);
    add(handList);
}

void GameGui::update(double delta) {
    Drawable::update(delta);
    menuRoot->update(delta);

    handList->setPos((renderer.window.getMousePos() - glm::ivec2(24)) / 3 * 3);

    renderer.window.setCursorHand(menuRoot->mouseActivity(renderer.window.getMousePos()));
    if (renderer.window.input.isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
        menuRoot->leftClickEvent(true, renderer.window.getMousePos());
    if (renderer.window.input.isMouseReleased(GLFW_MOUSE_BUTTON_LEFT))
        menuRoot->leftClickEvent(false, renderer.window.getMousePos());
    if (renderer.window.input.isMousePressed(GLFW_MOUSE_BUTTON_RIGHT))
        menuRoot->rightClickEvent(true, renderer.window.getMousePos());
    if (renderer.window.input.isMouseReleased(GLFW_MOUSE_BUTTON_RIGHT))
        menuRoot->rightClickEvent(false, renderer.window.getMousePos());
}

void GameGui::winResized(glm::ivec2 win) {
    this->win = win;

    builtIn.get<GuiRect>("crosshair")->setPos({win.x / 2 - 11, win.y / 2 - 9});
    builtIn.get<GuiRect>("viginette")->setScale(win);

    builder.build(win);
}

void GameGui::setMenu(const std::string& menu, const std::map<std::string, GuiBuilder::ComponentCallbacks>& callbacks) {
    menuState = "menu"; //TODO: Implement the menu state properly
    builder.setGui(menu, callbacks);
    builder.build(win);
}

void GameGui::closeMenu() {
    builder.clear();
    menuState = "";
}

const std::string &GameGui::getMenuState() {
    return menuState;
}

void GameGui::setVisible(bool visible) {
    GuiComponent::setVisible(visible);
    builtIn.setVisible(visible);
}

void GameGui::drawViginette(Renderer &renderer) {
    builtIn.draw(renderer);
}
