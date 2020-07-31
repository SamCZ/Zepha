//
// Created by aurailus on 2020-07-30.
//

#pragma once

#include <glm/vec3.hpp>

#include "../../def/item/SelectionBox.h"

class Entity {
public:
    virtual glm::vec3 getPos();
    virtual void setPos(glm::vec3 position);

    virtual glm::vec3 getVel();
    virtual void setVel(glm::vec3 velocity);

    virtual glm::vec3 getVisualOffset();
    virtual void setVisualOffset(glm::vec3 vs);

    virtual float getRotateX();
    virtual void setRotateX(float rotation);

    virtual float getRotateY();
    virtual void setRotateY(float rotation);

    virtual float getRotateZ();
    virtual void setRotateZ(float rotation);

    virtual glm::vec3 getScale();
    virtual void setScale(float scale);
    virtual void setScale(glm::vec3 scale);

protected:
    glm::vec3 pos {};
    glm::vec3 vel {};
    glm::vec3 rot {};
    glm::vec3 visualOff {};
    glm::vec3 scale {1, 1, 1};

    SelectionBox collision;
};
