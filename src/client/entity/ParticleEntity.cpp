#include "ParticleEntity.h"

#include "game/def/BlockDef.h"
#include "game/atlas/asset/AtlasTexture.h"

ParticleEntity::ParticleEntity(SubgamePtr game, DimensionPtr dim, vec3 pos, BlockDef& block) :
	DrawableEntity(game, dim, std::make_shared<Model>()), Entity(game, dim) {
	setPos(pos + vec3 { .5, .3, .5 });
	
	let it = block.model.textures.cbegin();
	advance(it, rand() % block.model.textures.size());
	
	vec4 uv = (*it)->getUVPos();
	
	f32 spanX = uv.z - uv.x;
	f32 spanY = uv.w - uv.y;
	
	f32 offX = spanX / 16 * (rand() % 12);
	f32 offY = spanX / 16 * (rand() % 12);
	
	uv -= vec4(0, 0, spanX * 0.75, spanY * 0.75);
	uv += vec4(offX, offY, offX, offY);
	
	vec<EntityVertex> vertices{
		{{ -0.05, -0.05, 0 }, { uv.x, uv.w, 0, 1 }, { 1, 1, 1 }, true, { 0, 0, 1 }, {}, {}},
		{{ -0.05, 0.05, 0 }, { uv.x, uv.y, 0, 1 }, { 1, 1, 1 }, true, { 0, 0, 1 }, {}, {}},
		{{ 0.05, 0.05, 0 }, { uv.z, uv.y, 0, 1 }, { 1, 1, 1 }, true, { 0, 0, 1 }, {}, {}},
		{{ 0.05, -0.05, 0 }, { uv.z, uv.w, 0, 1 }, { 1, 1, 1 }, true, { 0, 0, 1 }, {}, {}},
	};
	
	vec<u32> indices{
		0, 1, 2, 2, 3, 0,
		0, 2, 1, 2, 0, 3,
	};
	
	f32 dir = glm::radians(static_cast<f32>(rand() % 360));
	f32 xDir = sinf(dir);
	f32 zDir = cosf(dir);
	
	f32 horiScale = 0.5f + (rand() % 100 / 100.f);
	
	vel.y = 6.f;
	vel.x = xDir * 2.f * horiScale;
	vel.z = zDir * 2.f * horiScale;
	
	setPos(getPos() + vec3(xDir, 0, zDir) / 4.f);
	
	uptr<EntityMesh> mesh = make_unique<EntityMesh>();
	mesh->create(vertices, indices);
	model->fromMesh(std::move(mesh));
	setScale(0.75f + (rand() % 10 / 20.f));
}

void ParticleEntity::update(f64 delta, vec3 player) {
	time += delta;
	
	setPos(getPos() + vel * glm::vec3(static_cast<f32>(delta)));
	
	f32 angle = -glm::degrees(std::atan2(player.z - pos.z, player.x - pos.x)) + 90.f;
	setRotateY(angle);
	
	vel.y -= 32.f * delta;
}

void ParticleEntity::draw(Renderer& renderer) {
	DrawableEntity::draw(renderer);
}