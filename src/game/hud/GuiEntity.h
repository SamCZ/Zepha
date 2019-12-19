//
// Created by aurailus on 25/11/18.
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "../graph/meshtypes/GuiMesh.h"
#include "../graph/drawable/Drawable.h"

class GuiEntity : public Drawable {
public:
    GuiEntity() = default;
    explicit GuiEntity(std::shared_ptr<GuiMesh> mesh);
    void setMesh(std::shared_ptr<GuiMesh> mesh);

    void draw(Renderer& renderer) override;

    void setPos(glm::vec2 position);
    glm::vec2 getPos();

    void setDepth(float depth);
    float getDepth();

    void setRotation(glm::mat4 rotation);
    glm::mat4 getRotation();

    void setScale(float scale);
    void setScale(glm::vec2 scale);
    void setScale(glm::vec3 scale);
    glm::vec2 getScale();
    glm::vec3 getScale3();

    glm::mat4 getModelMatrix();

    void cleanup();
    ~GuiEntity() = default;
protected:
    glm::vec3 position {};
    glm::mat4 rotation {};
    glm::vec3 scale {1, 1, 1};

private:
    std::shared_ptr<GuiMesh> mesh = nullptr;
};

