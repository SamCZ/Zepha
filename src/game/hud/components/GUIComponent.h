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

    virtual void setPos(glm::ivec2 pos);
    virtual glm::ivec2 getPos();

    bool mouseActivity(glm::ivec2 pos);
    bool triggerClick(glm::ivec2 pos);
    void setClickCallback(std::function<void()> cb);

    void add(std::shared_ptr<GUIComponent> component);
    void insert(unsigned int index, std::shared_ptr<GUIComponent> component);
    template<class T> std::shared_ptr<T> get(const std::string &key) {
        for (auto &it : children) {
            if (it.get()->key == key) {
                return std::static_pointer_cast<T>(it);
            }
        }
    };
    void remove(const std::string& key);
    void empty();

    const std::string& getKey();

    void setVisible(bool visible) override;
    void draw(Renderer& renderer) override;
protected:
    std::string key = "";
    GUIComponent* parent = nullptr;
    std::list<std::shared_ptr<GUIComponent>> children;

    glm::ivec2 pos {};
    glm::vec2 scale {};
    glm::vec4 padding {};
    bool visible = true;

    std::function<void()> cb = nullptr;

    GuiEntity entity;
private:
    void updatePos();
};


