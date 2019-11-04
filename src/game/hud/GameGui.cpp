//
// Created by aurailus on 05/02/19.
//

#include "GameGui.h"
#include "../../util/Util.h"

const static float SCALE_MODIFIER = 3;

GameGui::GameGui(glm::vec2 bufferSize, TextureAtlas& atlas) :
    bufferSize(bufferSize),
    atlas(atlas) {

    auto crosshair = std::make_shared<GUIRect>("__crosshair");
    crosshair->create({22, 22}, {}, atlas.getTextureRef("crosshair"));
    crosshair->setPos({bufferSize.x / 2 - 11, bufferSize.y / 2 - 9});
    add(crosshair);

    auto viginette = std::make_shared<GUIRect>("__viginette");
    viginette->create(bufferSize, {}, atlas.getTextureRef("viginette"));
    add(viginette);

    auto root = std::make_shared<GUIContainer>("__root");
    add(root);
}

void GameGui::bufferResized(glm::vec2 bufferSize) {
    this->bufferSize = bufferSize;

    get<GUIRect>("__crosshair")->setPos({bufferSize.x / 2 - 11, bufferSize.y / 2 - 9});
    get<GUIRect>("__viginette")->setScale({bufferSize.x, bufferSize.y});
}

void GameGui::setMenu(const std::string& menu) {
    menuState = "aaa";

    std::vector<std::string> lines;
    {
        std::string::size_type pos = 0;
        std::string::size_type prev = 0;

        while ((pos = menu.find('\n', prev)) != std::string::npos) {
            std::string sub = menu.substr(prev, pos - prev);
            if (sub.substr(0, 2) != "--") {
                std::string::size_type start = sub.find_first_not_of("\t\v\r ");
                if (start == std::string::npos) start = 0;
                std::string::size_type end = sub.find_last_not_of("\t\v\r ");

                sub = sub.substr(start, end - start + 1);

                if (!sub.empty()) lines.push_back(sub);
            }
            prev = pos + 1;
        }
    }

    components.empty();
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

    get<GUIContainer>("__root")->empty();
    recursivelyCreate(components, get<GUIContainer>("__root"));
}

void GameGui::recursivelyCreate(std::vector<SerializedGuiElem> components, std::shared_ptr<GUIComponent> parent) {
    for (auto& data : components) {
        std::shared_ptr<GUIComponent> component = createComponent(data);
        parent->add(component);

        recursivelyCreate(data.children, component);
    }
}

std::shared_ptr<GUIComponent> GameGui::createComponent(SerializedGuiElem& data) {
    if (data.type == "body") {
        auto rect = std::make_shared<GUIRect>(data.key);
        rect->create(bufferSize, {}, {0, 0, 0, 0.2});
        return rect;
    }
    else if (data.type == "rect") {
        auto rect = std::make_shared<GUIRect>(data.key);

        glm::vec2 pos {};
        if (data.tokens.count("position")) pos = deserialize2DVector(data.tokens["position"]) * SCALE_MODIFIER;
        glm::vec2 size {};
        if (data.tokens.count("size")) size = deserialize2DVector(data.tokens["size"]) * SCALE_MODIFIER;
        glm::vec4 padding {};
        if (data.tokens.count("padding")) padding = deserialize4DVector(data.tokens["padding"]) * SCALE_MODIFIER;
        std::string background = "";
        if (data.tokens.count("background")) background = data.tokens["background"];

        size.x -= padding.y + padding.w;
        size.y -= padding.x + padding.z;

        if (background[0] == '#') rect->create(size, padding, Util::hexToColorVec(background));
        else if (background.substr(0, 6) == "asset(") rect->create(size, padding, atlas.getTextureRef(background.substr(7, background.length() - 7 - 2)));
        else rect->create(size, padding, glm::vec4 {});

        rect->setPos(pos);
        return rect;
    }
//    else if (type == "text") {
//        std::string posStr = element.get<std::string>("position");
//        std::string sizeStr = element.get<std::string>("size");
//        std::string padStr = element.get<std::string>("padding");
//
//        glm::vec2 pos = calc2dVec(posStr) * scale_modifier;
//        glm::vec2 scale = glm::vec2(scale_modifier);
//        glm::vec4 padding = calc4dVec(padStr) * scale_modifier;
//        glm::vec4 bgcolor = Util::hexToColorVec(element.get_or<std::string>("background_color", "#0000"));
//        glm::vec4 color = Util::hexToColorVec(element.get_or<std::string>("color", "#fff"));
//        std::string contents = element.get<std::string>("contents");
//
//        std::shared_ptr<GUIText> text = std::make_shared<GUIText>(key);
//        text->create(scale, padding, bgcolor, color, {defs.textures(), defs.textures().getTextureRef("font")});
//        text->setText(contents);
//        component = text;
//        text->setPos(pos);
//    }
}

void GameGui::closeMenu() {
    menuState = "";
    get<GUIContainer>("__root")->remove("__lua_root");
}

const std::string &GameGui::getMenuState() {
    return menuState;
}

void GameGui::setVisible(bool visible) {
    GUIComponent::setVisible(visible);
}

float GameGui::deserializeNumber(const std::string& input, bool height = false) {
    if (input.find("px") != std::string::npos) {
        return atof(input.substr(0, input.find("px")).c_str());
    }
    if (input.find('%') != std::string::npos) {
        return atof(input.substr(0, input.find('%')).c_str()) / 100 * (height ? bufferSize.y : bufferSize.x) / SCALE_MODIFIER;
    }
    return 0;
}

glm::vec2 GameGui::deserialize2DVector(const std::string& input) {
    glm::vec2 r;
    size_t count = std::count(input.begin(), input.end(), ' ');

    if (count == 0) { //1 value
        r.x = deserializeNumber(input);
        r.y = deserializeNumber(input, true);
        return r;
    }

    if (count == 1) { //2 values
        size_t delim = input.find(' ');
        std::string x = input.substr(0, delim);
        std::string y = input.substr(delim + 1);

        r.x = deserializeNumber(x);
        r.y = deserializeNumber(y, true);
        return r;
    }

    std::cout << Log::err << "Invalid dimension string " << input << Log::endl;
    return r;
}

glm::vec4 GameGui::deserialize4DVector(const std::string& input) {
    glm::vec4 r;
    size_t count = std::count(input.begin(), input.end(), ' ');

    if (count == 0) { //1 value
        r = glm::vec4(deserializeNumber(input));
        return r;
    }

    if (count == 1) { //2 values
        size_t delim = input.find(' ');
        std::string x = input.substr(0, delim);
        std::string y = input.substr(delim + 1);

        r.x = deserializeNumber(x);
        r.y = deserializeNumber(y);
        r.z = r.x;
        r.w = r.y;
        return r;
    }

    if (count == 3) { //4 values
        size_t begin = 0;
        for (int i = 0; i < 3; i++) {
            size_t end = input.find(' ', begin);
            r[i] = deserializeNumber(input.substr(begin, end - begin));
            begin = end + 1;
        }
        r[3] = deserializeNumber(input.substr(begin));
        return r;
    }

    std::cout << Log::err << "Invalid dimension string " << input << Log::endl;
    return r;
}