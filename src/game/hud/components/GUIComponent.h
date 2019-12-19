//
// Created by aurailus on 27/07/19.
//

#pragma once


#include <memory>
#include <list>
#include "../GuiEntity.h"

class GUIComponent : public Drawable {
public:
    typedef std::function<void(bool, glm::ivec2)> callback;

    GUIComponent() = default;
    explicit GUIComponent(const std::string& key);

    virtual void setScale(glm::vec2 scale);
    virtual glm::vec2 getScale();

    virtual void setPadding(glm::vec4 padding);
    virtual glm::vec4 getPadding();

    virtual void setPos(glm::ivec2 pos);
    virtual glm::ivec2 getPos();

    void setDepth(float depth);

    bool mouseActivity(glm::ivec2 pos);
    bool leftClickEvent(bool state, glm::ivec2 pos);
    bool rightClickEvent(bool state, glm::ivec2 pos);

    virtual void setHoverCallback(const callback& hoverCallback);
    virtual void setLeftClickCallback(const callback& leftClickCallback);
    virtual void setRightClickCallback(const callback& rightClickCallback);
    void setCallbacks(const callback& left, const callback& right, const callback& hover);

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

    bool hovered = false;
protected:
    bool clickEvent(bool left, bool state, glm::ivec2 pos);

    std::string key = "";
    GUIComponent* parent = nullptr;
    std::list<std::shared_ptr<GUIComponent>> children;

    bool visible = true;

    glm::ivec2 pos {};
    glm::vec2 scale {};
    glm::vec4 padding {};
    glm::ivec2 hitbox {};

    callback cbLeftClick = nullptr;
    callback cbRightClick = nullptr;
    callback cbHover = nullptr;

    GuiEntity entity;
private:
    void updatePos();
};


