#include "Collision.h"

#include "game/Subgame.h"
#include "world/dim/Dimension.h"
#include "game/def/BlockDef.h"
#include "game/atlas/DefinitionAtlas.h"
#include "game/def/mesh/SelectionBox.h"

bool Collision::isOnGround(SubgamePtr game, DimensionPtr dim,
	const SelectionBox& collisionBox, glm::vec3 pos, glm::vec3 vel) {
	constexpr static double INC = 0.05;
	
	pos.y -= INC;
	return vel.y <= 0 && collidesAt(game, dim, collisionBox, pos);
}

std::tuple<glm::vec3, glm::vec3> Collision::applyVel(SubgamePtr game, DimensionPtr dim,
	const SelectionBox& collisionBox, glm::vec3 pos, glm::vec3 vel, float delta) {
	constexpr static double INC = 0.05;
	
	for (unsigned char i = 0; i < 3; i++) {
		for (int j = 0; j < std::abs(vel[i] * delta) / INC; j++) {
			double moveAmount = std::max(std::min(INC, std::abs(vel[i] * delta) - (j * INC)), 0.);
			
			glm::vec3 newPos = pos;
			newPos[i] += moveAmount * (vel[i] < 0 ? -1 : 1);
			
			if (!collidesAt(game, dim, collisionBox, newPos)) pos = newPos;
			else if (i != 1 || vel[i] > 0) vel[i] = 0;
		}
	}
	
	return std::make_tuple(pos, vel);
}

bool Collision::collidesAt(SubgamePtr game, DimensionPtr dim, const SelectionBox& collisionBox, glm::vec3 pos) {
	SelectionBox worldAlignedCollisionBox = { collisionBox.a + pos, collisionBox.b + pos };
	glm::vec3 offset {};
	
	offset.x = collisionBox.a.x;
	while (true) {
		offset.y = collisionBox.a.y;
		while (true) {
			offset.z = collisionBox.a.z;
			while (true) {
				glm::vec3 offsetPos = glm::floor(pos + offset);
				auto& def = game->getDefs().blockFromId(dim->getBlock(offsetPos));
				
				if (def.solid) {
					for (auto& cBox : def.cBoxes) {
						SelectionBox blockBox = { cBox.a + offsetPos, cBox.b + offsetPos };
						
						if ((blockBox.a.x <= worldAlignedCollisionBox.b.x && blockBox.b.x >= worldAlignedCollisionBox.a.x) &&
						    (blockBox.a.y <= worldAlignedCollisionBox.b.y && blockBox.b.y >= worldAlignedCollisionBox.a.y) &&
						    (blockBox.a.z <= worldAlignedCollisionBox.b.z && blockBox.b.z >= worldAlignedCollisionBox.a.z))
							return true;
					}
				}
				
				if (offset.z == collisionBox.b.z) break;
				offset.z = (std::min)(std::floor(offset.z + 1), collisionBox.b.z);
			}
			if (offset.y == collisionBox.b.y) break;
			offset.y = (std::min)(std::floor(offset.y + 1), collisionBox.b.y);
		}
		if (offset.x == collisionBox.b.x) break;
		offset.x = (std::min)(std::floor(offset.x + 1), collisionBox.b.x);
	}
	
	return false;
}