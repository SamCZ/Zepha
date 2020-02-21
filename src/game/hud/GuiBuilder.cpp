//
// Created by aurailus on 2019-12-12.
//

#include "GuiBuilder.h"

#include "components/basic/GuiRect.h"
#include "components/basic/GuiText.h"
#include "components/basic/GuiModel.h"
#include "components/basic/GuiContainer.h"
#include "components/compound/GuiImageButton.h"

GuiBuilder::GuiBuilder(ClientGame& defs, std::shared_ptr<GuiContainer> root) :
        game(defs), root(root) {}

void GuiBuilder::setGui(const std::string& menu, const std::map<std::string, ComponentCallbacks>& callbacks) {
    this->callbacks = callbacks;
    deserialize(menu);
}

void GuiBuilder::deserialize(const std::string& menu) {
    // Split the lines by the newline delimiter
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
    unsigned int keyInd = 0;
    std::vector<SerialGui::Elem*> stack {};
    SerialGui::Elem* component = nullptr;

    // Parse through the serialized structure and create the Serialized tree
    for (const std::string& line : lines) {
        if (line.find(':') != std::string::npos) {
            // A Property
            if (component == nullptr) throw "expected a component name before a property";

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
        else if (line != "end") {
            // Beginning of a component definition
            std::string key = "";
            std::string::size_type keyStart;
            std::string::size_type keyEnd;
            if ((keyStart = line.find('[')) != std::string::npos &&
                (keyEnd = line.find_last_of(']')) != std::string::npos && keyEnd > keyStart) {
                key = line.substr(keyStart + 1, keyEnd - keyStart - 1);
            }
            else {
                // Create an implicit key because one was not specified
                key = "__" + std::to_string(keyInd++);
            }

            std::string type = line.substr(0, keyStart);
            SerialGui::Elem g {type, key, {}, {}};

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
        else {
            // End of a component definition -- pop the stack up
            if (stack.size() > 0) {
                component = stack[stack.size() - 1];
                stack.pop_back();
            }
            else component = nullptr;
        }
    }
}

void GuiBuilder::build(glm::ivec2 winBounds) {
    clear(false);
    recursivelyCreate(components, root, winBounds);
}

void GuiBuilder::clear(bool clrCallbacks) {
    if (clrCallbacks) callbacks.clear();
    root->empty();
}

void GuiBuilder::recursivelyCreate(std::vector<SerialGui::Elem> components, std::shared_ptr<GuiComponent> parent, glm::ivec2 bounds) {
    for (auto& data : components) {
        std::shared_ptr<GuiComponent> component = createComponent(data, bounds);
        if (component == nullptr) continue;
        parent->add(component);
        recursivelyCreate(data.children, component, bounds);
    }
}

std::shared_ptr<GuiComponent> GuiBuilder::createComponent(SerialGui::Elem& data, glm::ivec2 bounds) {
    std::shared_ptr<GuiComponent> c = nullptr;

    GuiComponent::callback cbLeftClick = nullptr;
    GuiComponent::callback cbRightClick = nullptr;
    GuiComponent::callback cbHover = nullptr;

    if (callbacks.count(data.key)) {
        cbLeftClick = callbacks[data.key].left;
        cbRightClick = callbacks[data.key].right;
        cbHover = callbacks[data.key].hover;
    }

    switch (Util::hash(data.type.c_str())) {
        default: break;
        case Util::hash("body"): {
            auto body = GuiRect::fromSerialized(data, game, bounds);
            body->setScale(bounds);
            c = body;
            break;
        }
        case Util::hash("rect"):
            c = GuiRect::fromSerialized(data, game, bounds);
            break;
        case Util::hash("button"):
            c = GuiImageButton::fromSerialized(data, game, bounds);
            break;
        case Util::hash("text"):
            c = GuiText::fromSerialized(data, game, bounds);
            break;
        case Util::hash("model"):
            c = GuiModel::fromSerialized(data, game, bounds);
            break;
    }

    if (c != nullptr) c->setCallbacks(cbLeftClick, cbRightClick, cbHover);
    return c;
}

GuiBuilder::~GuiBuilder() {
    clear(true);
}
