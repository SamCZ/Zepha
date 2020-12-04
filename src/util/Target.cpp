//
// Created by aurailus on 2020-07-27.
//

#include "Target.h"

#include "game/def/mesh/SelectionBox.h"

Target::Target(DimensionPtr dim, unsigned int id) :
	type(Type::ENTITY), dim(dim), data({ .entity = { id }}) {}
 
Target::Target(DimensionPtr dim, glm::ivec3 pos, EVec face) :
	type(Type::BLOCK), dim(dim), data({ .block = { pos, face }}) {}

glm::ivec3 Target::getAbovePos() const {
	if (type != Type::BLOCK) throw std::runtime_error("Attempt to getAbovePos on non-block target.");
	return glm::ivec3(data.block.pos) + SelectionBox::faceToOffset(data.block.face);
}