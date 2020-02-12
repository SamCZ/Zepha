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

    void setRotateX(float rotation);
    void interpRotateX(float rotation);
    float getRotateX();

    void setRotateY(float rotation);
    void interpRotateY(float rotation);
    float getRotateY();

    void setRotateZ(float rotation);
    void interpRotateZ(float rotation);
    float getRotateZ();

    void interpScale(float scale);
    void setScale(float scale);
    void interpScale(glm::vec3 scale);
    void setScale(glm::vec3 scale);
    glm::vec3 getScale();

    void setAnimations(const std::vector<AnimationSegment>& anims);
    void playAnimation(const std::string& anim, bool loop);
    void playRange(unsigned int start, unsigned int end, bool loop);
    void setPlaying(bool playing, unsigned int offset);

    void cleanup();
    ~Entity() override;

    AnimationState animState {};
protected:
    glm::mat4 getModelMatrix();

    glm::vec3 position {};
    glm::vec3 visualPosition {};

    glm::vec3 visualOffset {};
    glm::vec3 visualVisualOffset {};

    glm::vec3 rotation {};
    glm::vec3 visualRotation {};

    glm::vec3 scale {1, 1, 1};
    glm::vec3 visualScale {1, 1, 1};

    std::shared_ptr<Model> model = nullptr;
    std::vector<glm::mat4> transforms {};
};

