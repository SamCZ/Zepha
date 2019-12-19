//
// Created by aurailus on 24/08/19.
//

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct GuiVertex {
    GuiVertex(glm::vec3 position, glm::vec4 colorData, glm::vec3 colorBlend, float useTexture) :
        position(position),
        colorData(colorData),
        colorBlend(colorBlend),
        useTexture(useTexture) {}

    glm::vec3 position;
    glm::vec4 colorData;
    glm::vec3 colorBlend;
    float useTexture;
};

#define STRIDE_OFFSET_GUI(m) sizeof(struct GuiVertex), (void *)offsetof(struct GuiVertex, m)
