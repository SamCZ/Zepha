//
// Created by aurailus on 27/07/19.
//

#pragma once


#include <memory>
#include <list>
#include "../GuiEntity.h"

class GUIComponent : public Drawable {
public:
    GUIComponent() = default;
    explicit GUIComponent(const std::string& key);

    virtual void setScale(glm::vec2 scale);
    virtual glm::vec2 getScale();

    virtual void setPadding(glm::vec4 padding);
    virtual glm::vec4 getPadding();

    virtual void setPos(glm::vec2 pos);
    virtual glm::vec2 getPos();

    void add(std::shared_ptr<GUIComponent> component);
    template<class T> std::shared_ptr<T> get(const std::string &key) {
        for (auto &it : children) {
            if (it.get()->key == key) {
                return std::static_pointer_cast<T>(it);
            }
        }
    };
    void remove(std::string key);
    void empty();

    void setVisible(bool visible) override;
    void draw(Renderer& renderer) override;
protected:
    std::string key = "";
    GUIComponent* parent = nullptr;
    std::list<std::shared_ptr<GUIComponent>> children;

    glm::vec2 pos {};
    glm::vec2 scale {};
    glm::vec4 padding {};
    bool visible = true;

    GuiEntity entity;
private:
    void updatePos();
};


