//
// Created by aurailus on 25/11/18.
//

#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../graph/EntityMesh.h"
#include "../../graph/drawable/Drawable.h"
#include "../../../def/entity/Model.h"
#include "../../../def/entity/AnimationState.h"

class Entity : public Drawable {
public:
    Entity();
    explicit Entity(std::shared_ptr<Model> model);
    void setModel(std::shared_ptr<Model> model);

    void update(double delta) override;
    void draw(Renderer& renderer) override;

    void setPos(glm::vec3 position);
    glm::vec3 getPos();

    void setAngle(float angle);
    float getAngle();

    void setScale(float scale);
    void setScale(glm::vec3 scale);
    glm::vec3 getScale();

    void cleanup();
    ~Entity() override;

    AnimationState animState {};
protected:
    glm::mat4 getModelMatrix();

    glm::vec3 position {};
    glm::vec3 scale {1, 1, 1};
    float angle = 0;

    std::shared_ptr<Model> model = nullptr;
    std::vector<glm::mat4> transforms {};
};

