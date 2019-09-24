//
// Created by aurailus on 25/11/18.
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graph/meshtypes/GuiMesh.h"
#include "../graph/drawable/Drawable.h"

class GuiEntity : public Drawable {
public:
    GuiEntity() = default;
    explicit GuiEntity(GuiMesh* mesh);
    void setMesh(GuiMesh* mesh);

    void draw(Renderer& renderer) override;

    void setPos(glm::vec2 position);
    glm::vec2 getPos();

    void setAngle(float angle);
    float getAngle();

    void setScale(float scale);
    void setScale(glm::vec2 scale);
    glm::vec2 getScale();

    glm::mat4 getModelMatrix();

    void cleanup();
    ~GuiEntity() override;

protected:
    glm::vec2 position {0, 0};
    glm::vec2 scale {1, 1};
    GLfloat angle = 0;

private:
    GuiMesh* mesh = nullptr;
};

