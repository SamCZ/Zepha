//
// Created by aurailus on 2019-12-24.
//

#pragma once

#include <string>
#include <glm/vec3.hpp>

#include "../../util/net/Packet.h"

class ServerEntity {
public:
    explicit ServerEntity(unsigned int id) : id(id) {};

    void setPos(glm::vec3 position);
    glm::vec3 getPos();

    void setVisualOffset(glm::vec3 vs);
    glm::vec3 getVisualOffset();

    void setRotateX(float rotation);
    float getRotateX();

    void setRotateY(float rotation);
    float getRotateY();

    void setRotateZ(float rotation);
    float getRotateZ();

    void setScale(float scale);
    float getScale();

    void setAppearance(const std::string& displayMode, const std::string& displayArgument1, const std::string& displayArgument2);

    bool checkAndResetDirty();
    Packet createPacket(bool reliable = true);
protected:
    unsigned int id = 0;

    glm::vec3 position {};
    glm::vec3 visualOffset {};
    glm::vec3 rotation {};
    float scale = 1;

    std::string displayMode = "";
    std::string displayArgument1 = "";
    std::string displayArgument2 = "";

    bool dirty = true;
};

