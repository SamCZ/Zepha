//
// Created by aurailus on 05/02/19.
//

#include "GameGui.h"
#include "components/compound/GUIInventoryList.h"

const static float SCALE_MODIFIER = 3;

GameGui::GameGui(glm::vec2 bufferSize, LocalDefs& defs) :
    bufferSize(bufferSize),
    defs(defs),
    list(60, 12) {

    auto crosshair = std::make_shared<GUIRect>("crosshair");
    crosshair->create({22, 22}, {}, defs.textures().getTextureRef("crosshair"));
    crosshair->setPos({bufferSize.x / 2 - 11, bufferSize.y / 2 - 9});
    builtIn.add(crosshair);

    auto viginette = std::make_shared<GUIRect>("viginette");
    viginette->create(bufferSize, {}, defs.textures().getTextureRef("viginette"));
    builtIn.add(viginette);

    menuRoot = std::make_shared<GUIContainer>("__lua_root");
    add(menuRoot);
}

void GameGui::bufferResized(glm::vec2 bufferSize) {
    this->bufferSize = bufferSize;

    builtIn.get<GUIRect>("crosshair")->setPos({bufferSize.x / 2 - 11, bufferSize.y / 2 - 9});
    builtIn.get<GUIRect>("viginette")->setScale({bufferSize.x, bufferSize.y});

    menuRoot->empty();
    recursivelyCreate(components, menuRoot);
}

void GameGui::setMenu(const std::string& menu) {
    menuState = "menu"; //TODO: Implement the menu state properly

    std::vector<std::string> lines;
    {
        std::string::size_type pos = 0;
        std::string::size_type prev = 0;

        while ((pos = menu.find('\n', prev)) != std::string::npos) {
            std::string sub = menu.substr(prev, pos - prev);
            std::string::size_type start = sub.find_first_not_of("\t\v\r ");
            if (start == std::string::npos) start = 0;
            std::string::size_type end = sub.find_last_not_of("\t\v\r ");

            sub = sub.substr(start, end - start + 1);
            if (sub.substr(0, 2) != "--" && !sub.empty()) lines.push_back(sub);
            prev = pos + 1;
        }
    }

    components.clear();
    std::vector<SerializedGuiElem*> stack {};
    unsigned int missingKey = 0;
    SerializedGuiElem* component = nullptr;

    for (const std::string& line : lines) {
        if (line.find(':') != std::string::npos) {
            //Token
            if (component == nullptr) throw "Property before component.";

            std::string::size_type delimiter = line.find(':');

            std::string name = line.substr(0, delimiter);
            std::string::size_type start = name.find_first_not_of("\t\v\r ");
            if (start == std::string::npos) start = 0;
            std::string::size_type end = name.find_last_not_of("\t\v\r ");
            name = name.substr(start, end - start + 1);

            std::string value = line.substr(delimiter + 1, std::string::npos);
            start = value.find_first_not_of("\t\v\r ");
            if (start == std::string::npos) start = 0;
            end = value.find_last_not_of("\t\v\r ");
            value = value.substr(start, end - start + 1);

            component->tokens.emplace(name, value);
        }
        else if (line == "end") {
            //End Component Def
            if (stack.size() > 0) {
                component = stack[stack.size() - 1];
                stack.pop_back();
            }
            else {
                stack.pop_back();
                component = nullptr;
            }
        }
        else {
            //Component
            std::string key = "";
            std::string::size_type keyStart;
            std::string::size_type keyEnd;
            if ((keyStart = line.find('[')) != std::string::npos &&
                (keyEnd = line.find_last_of(']')) != std::string::npos && keyEnd > keyStart) {
                key = line.substr(keyStart + 1, keyEnd - keyStart - 1);
            }
            else {
                key = "__" + std::to_string(missingKey++);
            }
            std::string type = line.substr(0, keyStart);

            SerializedGuiElem g;
            g.key = key;
            g.type = type;
            g.tokens = {};
            g.children = {};

            if (component == nullptr) {
                components.push_back(std::move(g));
                component = &components[components.size() - 1];
            }
            else {
                component->children.push_back(std::move(g));
                if (component != nullptr) stack.push_back(component);
                component = &component->children[component->children.size() - 1];
            }
        }
    }

    menuRoot->empty();
    recursivelyCreate(components, menuRoot);
}

void GameGui::recursivelyCreate(std::vector<SerializedGuiElem> components, std::shared_ptr<GUIComponent> parent) {
    for (auto& data : components) {
        std::shared_ptr<GUIComponent> component = createComponent(data);
        parent->add(component);
        recursivelyCreate(data.children, component);
    }
}

std::shared_ptr<GUIComponent> GameGui::createComponent(SerializedGuiElem& data) {
    glm::vec2 pos {};

    if (data.tokens.count("position")) {
        auto tokens = splitValue(data.tokens["position"], 2);
        pos = {stringToNum(tokens[0], PercentBehavior::BUFF_WIDTH),
               stringToNum(tokens[1], PercentBehavior::BUFF_HEIGHT)};
    }

    glm::vec2 offset {};
    if (data.tokens.count("position_anchor")) {
        auto tokens = splitValue(data.tokens["position_anchor"], 2);
        offset = {stringToNum(tokens[0], PercentBehavior::DECIMAL),
               stringToNum(tokens[1], PercentBehavior::DECIMAL)};
    }

    glm::vec2 size {};
    if (data.tokens.count("size")) {
        auto tokens = splitValue(data.tokens["size"], 2);
        size = {stringToNum(tokens[0], PercentBehavior::BUFF_WIDTH),
                stringToNum(tokens[1], PercentBehavior::BUFF_HEIGHT)};
    }

    pos -= offset * size;

    glm::vec4 padding {};
    if (data.tokens.count("padding")) {
        auto tokens = splitValue(data.tokens["padding"], 4);
        padding = {stringToNum(tokens[0], PercentBehavior::BUFF_HEIGHT),
                   stringToNum(tokens[1], PercentBehavior::BUFF_WIDTH),
                   stringToNum(tokens[2], PercentBehavior::BUFF_HEIGHT),
                   stringToNum(tokens[3], PercentBehavior::BUFF_WIDTH)};
    }

    if (data.type == "body") {
        auto rect = std::make_shared<GUIRect>(data.key);
        rect->create(bufferSize, {}, {0, 0, 0, 0.2});
        return rect;
    }

    else if (data.type == "rect") {
        size.x -= padding.y + padding.w;
        size.y -= padding.x + padding.z;

        std::string background = "";
        if (data.tokens.count("background")) background = data.tokens["background"];

        auto rect = std::make_shared<GUIRect>(data.key);
        if (background[0] == '#') rect->create(size, padding, Util::hexToColorVec(background));
        else if (background.substr(0, 6) == "asset(") rect->create(size, padding, defs.textures().getTextureRef(background.substr(7, background.length() - 7 - 2)));
        else rect->create(size, padding, glm::vec4 {});

        rect->setPos(pos);
        return rect;
    }

    else if (data.type == "inventory") {
        glm::vec4 padding {};
        if (data.tokens.count("padding")) {
            auto tokens = splitValue(data.tokens["padding"], 4);
            padding = {stringToNum(tokens[0], PercentBehavior::BUFF_HEIGHT),
                       stringToNum(tokens[1], PercentBehavior::BUFF_WIDTH),
                       stringToNum(tokens[2], PercentBehavior::BUFF_HEIGHT),
                       stringToNum(tokens[3], PercentBehavior::BUFF_WIDTH)};
        }

        glm::vec2 innerPadding {};
        if (data.tokens.count("slot_spacing")) {
            auto tokens = splitValue(data.tokens["slot_spacing"], 2);
            innerPadding = {stringToNum(tokens[0], PercentBehavior::BUFF_HEIGHT),
                       stringToNum(tokens[1], PercentBehavior::BUFF_WIDTH)};
        }

        auto inv = std::make_shared<GUIInventoryList>(data.key);

        inv->create(glm::vec2(SCALE_MODIFIER), padding * SCALE_MODIFIER, innerPadding * SCALE_MODIFIER, list, defs);
        inv->setPos(pos);
        return inv;
    }

    else if (data.type == "text") {
        glm::vec2 scale = {1, 1};
        if (data.tokens.count("scale")) {
            auto tokens = splitValue(data.tokens["scale"], 2);
            scale = {stringToNum(tokens[0], PercentBehavior::DECIMAL),
                     stringToNum(tokens[1], PercentBehavior::DECIMAL)};
        }

        glm::vec4 padding {};
        if (data.tokens.count("padding")) {
            auto tokens = splitValue(data.tokens["padding"], 4);
            padding = {stringToNum(tokens[0], PercentBehavior::BUFF_HEIGHT),
                       stringToNum(tokens[1], PercentBehavior::BUFF_WIDTH),
                       stringToNum(tokens[2], PercentBehavior::BUFF_HEIGHT),
                       stringToNum(tokens[3], PercentBehavior::BUFF_WIDTH)};
        }

        glm::vec4 background_color = Util::hexToColorVec("#0000");
        if (data.tokens.count("background")) background_color = Util::hexToColorVec(data.tokens["background"]);
        glm::vec4 color = Util::hexToColorVec("#fff");
        if (data.tokens.count("color")) color = Util::hexToColorVec(data.tokens["color"]);

        std::string content = "Missing content string";
        if (data.tokens.count("content")) content = data.tokens["content"].substr(1, data.tokens["content"].size() - 2);

        std::string::size_type off = 0;
        while ((off = content.find("\\n", off)) != std::string::npos) {
            content.replace(off, 2, "\n");
            off += 1;
        }

        auto text = std::make_shared<GUIText>(key);
        text->create(scale * SCALE_MODIFIER, padding, background_color, color, {defs.textures(), defs.textures().getTextureRef("font")});
        text->setText(content);
        text->setPos(pos);
        return text;
    }
}

void GameGui::closeMenu() {
    menuRoot->empty();
    menuState = "";
}

const std::string &GameGui::getMenuState() {
    return menuState;
}

void GameGui::setVisible(bool visible) {
    GUIComponent::setVisible(visible);
    builtIn.setVisible(visible);
}

float GameGui::stringToNum(const std::string& input, PercentBehavior behavior = PercentBehavior::BUFF_WIDTH) {
    if (input.find("px") != std::string::npos) {
        return atof(input.substr(0, input.find("px")).c_str()) * SCALE_MODIFIER;
    }

    if (input.find('%') != std::string::npos) {
        float decimal = atof(input.substr(0, input.find('%')).c_str()) / 100;
        switch (behavior) {
            case PercentBehavior::DECIMAL:
                return decimal;
            case PercentBehavior::BUFF_WIDTH:
                return round(decimal * bufferSize.x / SCALE_MODIFIER) * SCALE_MODIFIER;
            case PercentBehavior::BUFF_HEIGHT:
                return round(decimal * bufferSize.y / SCALE_MODIFIER) * SCALE_MODIFIER;
        }
    }

    return atof(input.c_str());

    return 0;
}

std::vector<std::string> GameGui::splitValue(const std::string &value, unsigned int targetVals) {
    std::vector<std::string> vec {};

    if (value == "") throw "No values for splitValue.";

    size_t count = std::count(value.begin(), value.end(), ' ');
    if (count + 1 > targetVals) throw "Too many values for splitValue.";

    size_t begin = 0;
    for (int i = 0; i < count; i++) {
        size_t end = value.find(' ', begin);
        vec.push_back(value.substr(begin, end - begin));
        begin = end + 1;
    }
    vec.push_back(value.substr(begin));

    while (vec.size() < targetVals) {
        for (auto& v: vec) {
            vec.push_back(v);
            if (vec.size() >= targetVals) break;
        }
    }

    return std::move(vec);
}

void GameGui::drawViginette(Renderer &renderer) {
    builtIn.draw(renderer);
}
