//
// Created by aurailus on 2019-12-12.
//

#include "GuiBuilder.h"
#include "components/compound/GUIImageButton.h"

GuiBuilder::GuiBuilder(LocalDefs& defs, std::shared_ptr<GUIContainer> root) :
    defs(defs), root(root) {}

void GuiBuilder::setGui(const std::string& menu, const std::map<std::string, std::function<void(glm::ivec2)>>& callbacks) {
    this->menu = menu;
    this->callbacks = callbacks;

    deserialize();
}

void GuiBuilder::deserialize() {
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
}

void GuiBuilder::build(glm::ivec2 win) {
    this->win = win;

    clear(false);
    recursivelyCreate(components, root);
}

void GuiBuilder::clear(bool clrCallbacks) {
    if (clrCallbacks) callbacks.clear();
    root->empty();
}

void GuiBuilder::recursivelyCreate(std::vector<SerializedGuiElem> components, std::shared_ptr<GUIComponent> parent) {
    for (auto& data : components) {
        std::shared_ptr<GUIComponent> component = createComponent(data);
        parent->add(component);
        recursivelyCreate(data.children, component);
    }
}

std::shared_ptr<GUIComponent> GuiBuilder::createComponent(SerializedGuiElem& data) {
    glm::vec2 pos {};

    std::function<void(glm::ivec2)> callback = nullptr;
    if (callbacks.count(data.key)) {
        callback = callbacks[data.key];
    }

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

        std::string background = "";
        if (data.tokens.count("background")) background = data.tokens["background"];

        if (background[0] == '#') rect->create(win, {}, Util::hexToColorVec(background));
        else if (background.substr(0, 6) == "asset(") rect->create(win, {}, defs.textures[background.substr(6, background.length() - 7)]);
        else rect->create(win, {}, glm::vec4 {});

        rect->setClickCallback(callback);
        return rect;
    }

    else if (data.type == "rect") {
        size.x -= padding.y + padding.w;
        size.y -= padding.x + padding.z;

        std::string background = "";
        if (data.tokens.count("background")) background = data.tokens["background"];

        auto rect = std::make_shared<GUIRect>(data.key);
        if (background[0] == '#') rect->create(size, padding, Util::hexToColorVec(background));
        else if (background.substr(0, 6) == "asset(") rect->create(size, padding, defs.textures[background.substr(6, background.length() - 7)]);
        else rect->create(size, padding, glm::vec4 {});

        rect->setPos(pos);
        rect->setClickCallback(callback);
        return rect;
    }

    else if (data.type == "button") {
        size.x -= padding.y + padding.w;
        size.y -= padding.x + padding.z;

        std::string background = "";
        if (data.tokens.count("background")) background = data.tokens["background"];
        std::string background_hover = background;
        if (data.tokens.count("background_hover")) background_hover = data.tokens["background_hover"];

        auto button = std::make_shared<GUIImageButton>(data.key);
        button->create(size, padding, defs.textures[background.substr(6, background.length() - 7)], defs.textures[background_hover.substr(6, background_hover.length() - 7)]);

        std::string content = "";
        if (data.tokens.count("content")) content = data.tokens["content"].substr(1, data.tokens["content"].size() - 2);

        if (content != "") {
            std::string::size_type off = 0;
            while ((off = content.find("\\n", off)) != std::string::npos) {
                content.replace(off, 2, "\n");
                off += 1;
            }

            auto text = std::make_shared<GUIText>(data.key + "__TEXT");
            text->create(glm::vec2(SCALE_MODIFIER), padding, {}, {1, 1, 1, 1},
                         {defs.textures, defs.textures["font"]});
            text->setText(content);
            text->setPos({6*SCALE_MODIFIER, size.y / 2 - 4.5*SCALE_MODIFIER});
            button->add(text);
        }

        button->setPos(pos);
        button->setClickCallback(callback);
        return button;
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

        auto text = std::make_shared<GUIText>(data.key);
        text->create(scale * SCALE_MODIFIER, padding, background_color, color, {defs.textures, defs.textures["font"]});
        text->setText(content);
        text->setPos(pos);
        text->setClickCallback(callback);
        return text;
    }

    return nullptr;
}
float GuiBuilder::stringToNum(const std::string& input, PercentBehavior behavior = PercentBehavior::BUFF_WIDTH) {
    if (input.find("px") != std::string::npos) {
        return atof(input.substr(0, input.find("px")).c_str()) * SCALE_MODIFIER;
    }

    if (input.find('%') != std::string::npos) {
        float decimal = atof(input.substr(0, input.find('%')).c_str()) / 100;
        switch (behavior) {
            case PercentBehavior::DECIMAL:
                return decimal;
            case PercentBehavior::BUFF_WIDTH:
                return round(decimal * win.x / SCALE_MODIFIER) * SCALE_MODIFIER;
            case PercentBehavior::BUFF_HEIGHT:
                return round(decimal * win.y / SCALE_MODIFIER) * SCALE_MODIFIER;
        }
    }

    return atof(input.c_str());

    return 0;
}

std::vector<std::string> GuiBuilder::splitValue(const std::string &value, unsigned int targetVals) {
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

GuiBuilder::~GuiBuilder() {
    clear(true);
}
