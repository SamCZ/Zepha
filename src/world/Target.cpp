//
// Created by aurailus on 2020-07-27.
//

#include "Target.h"

#include "../def/item/SelectionBox.h"

Target::Target(glm::ivec3 pos, EVec face) : type(Type::BLOCK), pos(pos), face(face) {}

glm::ivec3 Target::getAbovePos() const {
    return glm::ivec3(pos) + SelectionBox::faceToOffset(face);
}
