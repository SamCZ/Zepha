//
// Created by aurailus on 25/11/18.
//

#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../graph/EntityMesh.h"
#include "../../graph/drawable/Drawable.h"

class Entity : public Drawable {
public:
    Entity();
    explicit Entity(EntityMesh* mesh);

    void setMesh(EntityMesh* mesh);

    void draw(Renderer& renderer) override;

    void setPos(glm::vec3 position);
    glm::vec3 getPos();

    void setAngle(float angle);
    float getAngle();

    void setScale(float scale);
    void setScale(glm::vec3 scale);
    glm::vec3 getScale();

    glm::mat4 getModelMatrix();

    void cleanup();
    ~Entity() override;

protected:
    glm::vec3 position {0, 0, 0};
    glm::vec3 scale {1, 1, 1};
    GLfloat angle = 0;

private:
    EntityMesh* mesh = nullptr;
};

