//
// Created by aurailus on 25/11/18.
//

#pragma once

#include <vector>
#include <memory>
#include <glm/mat4x4.hpp>

#include "Model.h"
#include "Entity.h"
#include "../graph/drawable/Drawable.h"

#include "AnimationState.h"
#include "AnimationSegment.h"

class DrawableEntity : virtual public Entity, public Drawable {
public:
    DrawableEntity();
    DrawableEntity(const DrawableEntity& o) = delete;
    explicit DrawableEntity(std::shared_ptr<Model> model);

    void update(double delta) override;
    void setModel(std::shared_ptr<Model> model);

    virtual void setPos(glm::vec3 position) override;
    virtual void interpPos(glm::vec3 position);

    virtual void setVisualOffset(glm::vec3 vs) override;
    virtual void interpVisualOffset(glm::vec3 vs);

    virtual void setRotateX(float rotation) override;
    virtual void interpRotateX(float rotation);

    virtual void setRotateY(float rotation) override;
    virtual void interpRotateY(float rotation);

    virtual void setRotateZ(float rotation) override;
    virtual void interpRotateZ(float rotation);

    virtual void setScale(float scale) override;
    virtual void interpScale(float scale);
    virtual void setScale(glm::vec3 scale) override;
    virtual void interpScale(glm::vec3 scale);

    void setAnimations(const std::vector<AnimationSegment>& anims);
    void playAnimation(const std::string& anim, bool loop);
    void playRange(unsigned int start, unsigned int end, bool loop);
    void setPlaying(bool playing, unsigned int offset);

    void draw(Renderer& renderer) override;

    void cleanup();
    ~DrawableEntity() override;

    AnimationState animState {};
    DrawableEntity* parent = nullptr;
protected:
    glm::mat4 getModelMatrix();
    glm::mat4 getRotationMatrix();

    glm::vec3 visualPosition {};
    glm::vec3 visualVisualOffset {};
    glm::vec3 visualRotation {};
    glm::vec3 visualScale {1, 1, 1};

    std::shared_ptr<Model> model;
    std::vector<glm::mat4> transforms {};
};
