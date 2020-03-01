//
// Created by aurailus on 27/07/19.
//

#pragma once


#include <list>
#include <memory>

#include "../../entity/Entity.h"

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

    bool mouseActivity(glm::ivec2 pos);
    void leftClickEvent(bool state, glm::ivec2 pos);
    void rightClickEvent(bool state, glm::ivec2 pos);

    template<class T> std::shared_ptr<T> get(const std::string &key) {
        for (auto &it : children) {
            if (it.get()->key == key) {
                return std::static_pointer_cast<T>(it);
            }
        }
    };

    void insert(unsigned int index, std::shared_ptr<GuiComponent> component);
    void add(std::shared_ptr<GuiComponent> component);

    void remove(const std::string& key);
    void empty();

    void draw(Renderer& renderer) override;
protected:
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

    Entity entity;
    std::array<callback, 3> callbacks;

private:
    void updatePos();
};


