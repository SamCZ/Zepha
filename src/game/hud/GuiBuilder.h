//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include <string>
#include <algorithm>
#include "SerializedGuiElem.h"
#include "../../def/LocalDefs.h"
#include "components/basic/GUIRect.h"
#include "components/basic/GUIText.h"
#include "components/basic/GUIContainer.h"

class GuiBuilder {
public:
    GuiBuilder(LocalDefs& defs, std::shared_ptr<GUIContainer> root);
    void setGui(const std::string& menu, const std::map<std::string, std::function<void(glm::ivec2)>>& callbacks = {});
    void build(glm::ivec2 win);
    void clear(bool clrCallbacks = true);

    ~GuiBuilder();

protected:
    const float SCALE_MODIFIER = 3;

    enum PercentBehavior {
        BUFF_WIDTH,
        BUFF_HEIGHT,
        DECIMAL
    };

    void deserialize();
    void recursivelyCreate(std::vector<SerializedGuiElem> components, std::shared_ptr<GUIComponent> parent);
    virtual std::shared_ptr<GUIComponent> createComponent(SerializedGuiElem& component);

    static std::vector<std::string> splitValue(const std::string& value, unsigned int targetVals = 0);
    float stringToNum(const std::string& input, PercentBehavior behavior);

    glm::ivec2 win;

    std::string menu;
    std::map<std::string, std::function<void(glm::ivec2)>> callbacks;

    std::shared_ptr<GUIContainer> root;
    std::vector<SerializedGuiElem> components {};
    LocalDefs& defs;
};
