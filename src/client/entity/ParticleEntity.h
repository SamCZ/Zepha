#pragma once

#include "util/Types.h"
#include "world/dim/ent/DrawableEntity.h"

class BlockDef;

class ParticleEntity : public DrawableEntity {
public:
	ParticleEntity(SubgamePtr game, DimensionPtr dim, vec3 pos, BlockDef& block);
	
	void update(f64 delta, vec3 player);
	
	void draw(Renderer& renderer) override;
	
	f32 time = 0;

private:
	vec3 vel;
};

