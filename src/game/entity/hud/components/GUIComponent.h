//
// Created by aurailus on 27/07/19.
//

#ifndef ZEUS_GUICOMPONENT_H
#define ZEUS_GUICOMPONENT_H


#include <memory>
#include <unordered_map>
#include "../../Entity.h"

class GUIComponent : Drawable {
public:
    GUIComponent() = default;
    GUIComponent(const std::string& key, GUIComponent* parent);

    virtual void setScale(glm::vec2 scale);
    virtual glm::vec2 getScale();

    virtual void setPadding(glm::vec4 padding);
    virtual glm::vec4 getPadding();

    virtual void setPos(glm::vec2 pos);
    virtual glm::vec2 getPos();

    void set(std::string key, std::shared_ptr<GUIComponent> component);
    void remove(std::string key);
    std::shared_ptr<GUIComponent> operator[](std::string);

    void setVisible(bool visible) override;

    void draw(Renderer& renderer) override;
protected:
    std::string key = "";
    GUIComponent* parent = nullptr;
    std::unordered_map<std::string, std::shared_ptr<GUIComponent>> children;

    glm::vec2 pos {};
    glm::vec2 scale {};
    glm::vec4 padding {};
    bool visible = true;

    Entity entity;
};


#endif //ZEUS_GUICOMPONENT_H
