//
// Created by aurailus on 25/11/18.
//

#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graph/meshtypes/EntityMesh.h"
#include "../graph/drawable/Drawable.h"
#include "Model.h"
#include "AnimationState.h"

class Entity : public Drawable {
public:
    Entity();
    explicit Entity(std::shared_ptr<Model> model);
    void setModel(std::shared_ptr<Model> model);

    void update(double delta) override;
    void draw(Renderer& renderer) override;

    void interpPos(glm::vec3 position);
    void setPos(glm::vec3 position);
    glm::vec3 getPos();

    void interpVisualOffset(glm::vec3 vs);
    void setVisualOffset(glm::vec3 vs);
    glm::vec3 getVisualOffset();

    //TODO: Remove these
    void interpAngle(float angle);
    void setAngle(float angle);
    float getAngle();

    void setRotation(glm::mat4 rotation);
    glm::mat4 getRotation();

    void interpScale(float scale);
    void setScale(float scale);
    float getScale();

    void cleanup();
    ~Entity() override;

    AnimationState animState {};
protected:
    glm::mat4 getModelMatrix();

    glm::vec3 position {};
    glm::vec3 visualOffset {};
    glm::mat4 rotation;
    float angle = 0;
    float scale = 1;

    glm::vec3 visualPosition {};
    glm::vec3 visualVisualOffset {};
    float visualScale = 1;
    float visualAngle = 0;

    std::shared_ptr<Model> model = nullptr;
    std::vector<glm::mat4> transforms {};
};

