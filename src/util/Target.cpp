//
// Created by aurailus on 2020-07-27.
//

#include "Target.h"

#include "game/def/mesh/SelectionBox.h"

Target::Target(DimensionPtr dim, glm::ivec3 pos, EVec face) :
	type(Type::BLOCK), dim(dim), pos(pos), face(face) {}

glm::ivec3 Target::getAbovePos() const {
	return glm::ivec3(pos) + SelectionBox::faceToOffset(face);
}
