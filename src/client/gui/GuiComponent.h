//
// Created by aurailus on 27/07/19.
//

#pragma once


#include <list>
#include <memory>
#include <functional>

#include "world/dim/ent/DrawableEntity.h"

class Window;

class GuiComponent : public Drawable {
public:
    enum class CallbackType { PRIMARY, SECONDARY, HOVER };
    typedef std::function<void(bool, glm::ivec2)> callback;

    GuiComponent() = default;
    explicit GuiComponent(const std::string& key);

    virtual void update(double delta) override;

    const std::string& getKey();

    virtual glm::ivec2 getPos();
    virtual void setPos(glm::ivec2 pos);
    virtual glm::vec2 getScale();
    virtual void setScale(glm::vec2 scale);
    virtual glm::vec4 getPadding();
    virtual void setPadding(glm::vec4 padding);
    void setOverflows(bool overflows);
    void setVisible(bool visible) override;

    virtual void setCallback(CallbackType type, const callback& cb);
    void handleMouseInput(Window& window);

    template<class T> std::shared_ptr<T> get(const std::string &key) {
        for (auto &it : children) {
            if (it.get()->key == key) {
                return std::static_pointer_cast<T>(it);
            }
        }
        return nullptr;
    };

    std::shared_ptr<GuiComponent> insert(unsigned int index, std::shared_ptr<GuiComponent> component);
    std::shared_ptr<GuiComponent> add(std::shared_ptr<GuiComponent> component);

    std::list<std::shared_ptr<GuiComponent>> getChildren();

    void remove(const std::string& key);
    void empty();

    void draw(Renderer& renderer) override;
protected:
    bool mouseActivity(glm::ivec2 pos);
    bool clickEvent(bool left, bool state, glm::ivec2 pos);

    std::string key = "";
    GuiComponent* parent = nullptr;
    std::list<std::shared_ptr<GuiComponent>> children;

    glm::ivec2 pos {};
    glm::vec2 scale {};
    glm::vec4 padding {};
    glm::ivec2 hitbox {};

    bool visible = true;
    bool hovered = false;
    bool overflows = false;

    DrawableEntity entity;
    std::array<callback, 3> callbacks;

private:
    void updatePos();
};


