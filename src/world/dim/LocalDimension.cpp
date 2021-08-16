#include "LocalDimension.h"

#include "world/LocalWorld.h"
#include "util/net/NetField.h"
#include "util/net/PacketView.h"
#include "lua/usertype/Target.h"
#include "lua/usertype/Player.h"
#include "world/dim/chunk/Chunk.h"
#include "client/graph/Renderer.h"
#include "world/dim/chunk/Region.h"
#include "world/dim/chunk/MapBlock.h"
#include "client/stream/MeshGenStream.h"
#include "client/graph/mesh/MeshChunk.h"

LocalDimension::LocalDimension(SubgamePtr game, LocalWorld& world,
	const string& identifier, u16 ind, sptr<MapGen> mapGen) :
	Dimension(game, static_cast<World&>(world), identifier, ind, std::move(mapGen)),
	meshGenStream(std::make_shared<MeshGenStream>(game, *this)) {}


/**
 * Deactivates a dimension, cleaning up all mesh chunks.
 * Called on the active dimension when the player leaves it.
 */

void LocalDimension::deactivate() {
	while (!renderElems.empty()) {
		auto chunk = renderElems.front();
		renderRefs.erase(chunk->getPos());
		renderElems.pop_front();
	}
	
	while (!regions.empty()) {
		removeRegion(regions.begin()->first);
	}
}

void LocalDimension::update(f64 delta) {
	finishMeshes();
	
	/* Update local entities and player entities. */
	for (auto& entity : entities) entity.entity.l()->update(delta);
	for (auto& entity : playerEntities) entity.update(delta);
	
	/*
	 * Delete mapblocks and regions that are outside of the retain range,
	 * and compress chunks if they are idle.
	 */
	
	let clientMapBlock = Space::MapBlock::world::fromBlock(static_cast<LocalWorld&>(world).getPlayer()->getPos());
	
	for (let it = regions.cbegin(); it != regions.cend();) {
		for (u16 m = 0; m < 64; m++) {
			let mapBlock = it->second->get(m);
			if (!mapBlock || (mapBlock->pos.y - mapBlockScanY) % MAPBLOCK_SCAN_Y_INTERVAL != 0) continue;
			
			if (abs(clientMapBlock.x - mapBlock->pos.x) > retainMapBlockRange.x ||
			    abs(clientMapBlock.y - mapBlock->pos.y) > retainMapBlockRange.y ||
			    abs(clientMapBlock.z - mapBlock->pos.z) > retainMapBlockRange.x) {
				
				for (u16 c = 0; c < 64; c++) {
					let chunk = mapBlock->get(c);
					if (chunk) removeMeshChunk(chunk->getPos());
				}
				
				it->second->remove(m);
				if (it->second->count <= 0) goto erase_region_and_continue;
			}
			else {
				for (u16 c = 0; c < 64; c++) {
					let chunk = mapBlock->get(c);
					if (chunk) chunk->compressIfIdle();
				}
			}
		}
		
		it++;
		continue;
		
		erase_region_and_continue:
		it = regions.erase(it);
	}
	
	mapBlockScanY = (mapBlockScanY + 1) % MAPBLOCK_SCAN_Y_INTERVAL;
}

void LocalDimension::setChunk(sptr<Chunk> chunk) {
	let clientMapBlock = Space::MapBlock::world::fromBlock(static_cast<LocalWorld&>(world).getPlayer()->getPos());
	let mapBlockPos = Space::MapBlock::world::fromChunk(chunk->getPos());
	
	if (abs(clientMapBlock.x - mapBlockPos.x) > retainMapBlockRange.x ||
		abs(clientMapBlock.y - mapBlockPos.y) > retainMapBlockRange.y ||
		abs(clientMapBlock.z - mapBlockPos.z) > retainMapBlockRange.x) return;
	
	Dimension::setChunk(chunk);
	meshChunk(chunk);
}

bool LocalDimension::setBlock(ivec3 pos, u16 block) {
	bool modified = Dimension::setBlock(pos, block);
	if (!modified) return false;
	
	let chunkPos = Space::Chunk::world::fromBlock(pos);
	let chunk = getChunk(chunkPos);
	
	chunk->setDirty(true);
	
	let lp = Space::Block::relative::toChunk(pos);
	let cp = Space::Chunk::world::fromBlock(pos);
	
	sptr<Chunk> adjacent;
	if (lp.x == 15 && (adjacent = getChunk(cp + ivec3 { 1, 0, 0 }))) adjacent->setDirty(true);
	else if (lp.x == 0 && (adjacent = getChunk(cp + ivec3 { -1, 0, 0 }))) adjacent->setDirty(true);
	if (lp.y == 15 && (adjacent = getChunk(cp + ivec3 { 0, 1, 0 }))) adjacent->setDirty(true);
	else if (lp.y == 0 && (adjacent = getChunk(cp + ivec3 { 0, -1, 0 }))) adjacent->setDirty(true);
	if (lp.z == 15 && (adjacent = getChunk(cp + ivec3 { 0, 0, 1 }))) adjacent->setDirty(true);
	else if (lp.z == 0 && (adjacent = getChunk(cp + ivec3 { 0, 0, -1 }))) adjacent->setDirty(true);
	
	meshChunk(chunk, true);
	return true;
}

void LocalDimension::blockPlace(const Target& target, PlayerPtr player) {
	std::tuple<sol::optional<Api::Usertype::ItemStack>, sol::optional<glm::vec3>> res = game->getParser().safe_function(
		game->getParser().core["block_place"], Api::Usertype::LocalPlayer(player.l()), Api::Usertype::Target(target));
	
	static_cast<LocalWorld&>(world).getNet().blockPlace(target);
	
	auto stack = std::get<sol::optional<Api::Usertype::ItemStack>>(res);
	if (!stack) return;
	
	auto inv = player.l()->getInventory();
	if (inv->hasList(player->getWieldList()))
		inv->getList(player->getWieldList())->setStack(player->getWieldIndex(), ItemStack(*stack, game));
}

void LocalDimension::blockInteract(const Target& target, PlayerPtr player) {
	game->getParser().safe_function(game->getParser().core["block_interact"],
		Api::Usertype::LocalPlayer(player.l()), Api::Usertype::Target(target));
	
	static_cast<LocalWorld&>(world).getNet().blockInteract(target);
}

void LocalDimension::blockPlaceOrInteract(const Target& target, PlayerPtr player) {
	std::tuple<sol::optional<Api::Usertype::ItemStack>, sol::optional<vec3>> res = game->getParser().safe_function(
		game->getParser().core["block_interact_or_place"], Api::Usertype::LocalPlayer(player.l()),
		Api::Usertype::Target(target));
	
	static_cast<LocalWorld&>(world).getNet().blockPlaceOrInteract(target);
	
	auto stack = std::get<sol::optional<Api::Usertype::ItemStack>>(res);
	if (!stack) return;
	
	auto inv = player.l()->getInventory();
	if (inv->hasList(player->getWieldList()))
		inv->getList(player->getWieldList())->setStack(player->getWieldIndex(), ItemStack(*stack, game));
}

double LocalDimension::blockHit(const Target& target, PlayerPtr player) {
	f64 timeout = 0, damage = 0;
	sol::tie(damage, timeout) = game->getParser().safe_function(game->getParser().core["block_hit"],
		Api::Usertype::LocalPlayer(player.l()), Api::Usertype::Target(target));
	
	static_cast<LocalWorld&>(world).getNet().blockHit(target);
	
	return timeout;
}

void LocalDimension::wieldItemUse(const Target& target, PlayerPtr player) {
	sol::optional<Api::Usertype::ItemStack> stack = game->getParser().safe_function(
		game->getParser().core["item_use"], Api::Usertype::LocalPlayer(player.l()), Api::Usertype::Target(target));
	
	static_cast<LocalWorld&>(world).getNet().wieldItemUse(target);
	
	auto inv = player->getInventory();
	if (stack && inv->hasList(player->getWieldList()))
		inv->getList(player->getWieldList())->setStack(player->getWieldIndex(), ItemStack(*stack, game));
}

void LocalDimension::setMeshChunk(sptr<MeshChunk> meshChunk) {
	if (renderRefs.count(meshChunk->getPos())) removeMeshChunk(meshChunk->getPos());
	renderElems.push_back(std::static_pointer_cast<ChunkRenderElem>(meshChunk));
	renderRefs.emplace(meshChunk->getPos(), --renderElems.end());
}

void LocalDimension::removeMeshChunk(const ivec3& pos) {
	if (!renderRefs.count(pos)) return;
	auto refIter = renderRefs.at(pos);
	
	if (!refIter->get()->updateChunkUse(pos, false)) {
		renderElems.erase(refIter);
		renderRefs.erase(pos);
	}
}

i64 LocalDimension::nextEntityInd() {
	return entityInd--;
}

void LocalDimension::addLocalEntity(Api::Usertype::Entity entity) {
	i64 id = entity.get_id();
	entities.push_back(entity);
	entityRefs.emplace(id, --entities.end());
}

void LocalDimension::removeLocalEntity(Api::Usertype::Entity entity) {
	i64 id = entity.get_id();
	
	if (!entityRefs.count(id)) return;
	auto refIter = entityRefs.at(id);
	
	entities.erase(refIter);
	entityRefs.erase(id);
}

void LocalDimension::serverEntitiesInfo(Deserializer& e) {
	e.read<u16>();
	
	while (!e.atEnd()) {
		string dat = e.read<string>();
		Deserializer d(dat);
		
		auto id = d.read<i64>();
		std::shared_ptr<LocalLuaEntity> activeEntity;
		if (entityRefs.count(id)) activeEntity = entityRefs.at(id)->entity.l();
		else {
			auto ent = make_shared<LocalLuaEntity>(game, world.getDimension(getInd()));
			auto entity = Api::Usertype::Entity(ent);
			ent->setId(id);
			entities.push_back(entity);
			entityRefs.emplace(id, --entities.end());
			activeEntity = ent;
		}
		
		while (!d.atEnd()) {
			const auto field = d.read<NetField>();
			switch (field) {
			default: {
				std::cout << Log::err << "Entity received unhandled NetField, Type "
				          << static_cast<int>(field) << "." << Log::endl;
				break;
			}
			
			case NetField::POS: {
				activeEntity->setPos(d.read<vec3>());
				break;
			}
			
			case NetField::VEL: {
				activeEntity->setVel(d.read<vec3>());
				break;
			}
			
			case NetField::ROT: {
				activeEntity->setRot(d.read<vec3>());
				break;
			}
			
			case NetField::SCALE: {
				activeEntity->setScale(d.read<vec3>());
				break;
			}
			
			case NetField::VISUAL_OFF: {
				activeEntity->setVisualOffset(d.read<vec3>());
				break;
			}
			
			case NetField::DISPLAY: {
				string mode, argA, argB;
				d.read(mode).read(argA).read(argB);
				activeEntity->setAppearance(mode, argA, argB);
				break;
			}
			
			case NetField::ANIM_STATE: {
				bool playing = d.read<bool>();
				activeEntity->animation.setPlaying(playing);
				break;
			}
			
			case NetField::ANIM_RANGE: {
				uvec2 range = d.read<uvec2>();
				bool loops = d.read<bool>();
				activeEntity->animation.setAnim(range, 10, loops);
				break;
			}
			
			case NetField::DIM: {
				activeEntity->setDim(world.getDimension(d.read<u16>()));
				break;
			}
			
			case NetField::COLLISION_BOX: {
				bool hasCollisionBox = d.read<bool>();
				if (hasCollisionBox) {
					vec3 a = d.read<vec3>();
					vec3 b = d.read<vec3>();
					activeEntity->setCollisionBox({ a, b });
				}
				else activeEntity->removeCollisionBox();
				break;
			}
			
			case NetField::COLLIDES: {
				activeEntity->setCollides(d.read<bool>());
				break;
			}
			
			case NetField::GRAVITY: {
				activeEntity->setGravity(d.read<f32>());
				break;
			}
			}
		}
	}
}

void LocalDimension::removeServerEntities(Deserializer& d) {
	d.read<u16>();
	while (!d.atEnd()) {
		i64 id = d.read<i64>();
		if (!entityRefs.count(id)) continue;
		auto refIter = entityRefs.at(id);
		entities.erase(refIter);
		entityRefs.erase(id);
	}
}

vec<Api::Usertype::Entity> LocalDimension::getEntitiesInRadius(vec3 pos, f32 radius) {
	vec<Api::Usertype::Entity> found {};
	for (auto& entity : entities) if (glm::distance(pos, entity.entity->getPos()) <= radius) found.push_back(entity);
	return found;
}

Api::Usertype::Entity& LocalDimension::getEntityById(i64 id) {
	return *entityRefs.at(id);
}

void LocalDimension::renderChunks(Renderer& renderer) {
	lastMeshesDrawn = 0;
	for (auto& renderElement : renderElems) {
		FrustumAABB bbox(renderElement->getPos() * vec3(16), vec3(16));
		if (renderer.camera.inFrustum(bbox) != Frustum::Intersection::OUTSIDE) {
			renderElement->draw(renderer);
			lastMeshesDrawn++;
		}
	}
}

void LocalDimension::renderEntities(Renderer& renderer) {
	for (auto& entity : entities) entity.entity.l()->draw(renderer);
	for (auto& entity : playerEntities) entity.draw(renderer);
}

u32 LocalDimension::getMeshChunksDrawn() {
	return lastMeshesDrawn;
}

u32 LocalDimension::getMeshChunksCommitted() {
	return lastMeshesCommitted;
}

std::unordered_set<ivec3, Vec::ivec3> LocalDimension::propogateAddNodes() {
	auto updated = Dimension::propogateAddNodes();
	for (auto& update : updated) meshChunk(getChunk(update));
	return {};
}

std::unordered_set<ivec3, Vec::ivec3> LocalDimension::propogateRemoveNodes() {
	auto updated = Dimension::propogateRemoveNodes();
	for (auto& update : updated) meshChunk(getChunk(update));
	return {};
}

void LocalDimension::finishMeshes() {
	lastMeshesCommitted = 0;
	auto finishedMeshes = meshGenStream->update();
	
	for (MeshChunkDetails* details : finishedMeshes) {
		if (!details->vertices.empty()) {
			setMeshChunk(make_shared<MeshChunk>(details->pos, details->vertices, details->indices));
			lastMeshesCommitted++;
		}
		else removeMeshChunk(details->pos);
		
		delete details;
	}
}

void LocalDimension::meshChunk(const sptr<Chunk>& chunk, bool priority, bool updateAdjacents) {
	// Run this loop first, because even if this chunk shouldn't render, the adjacents maybe should.
	bool render = true;
	for (let dir : Vec::TO_VEC)
		if (!adjacentExists(chunk->getPos() + dir, updateAdjacents, priority))
			render = false;
	if (!render) return;
	
	if (!chunk->isDirty()) return;
	if (!chunk->chunkShouldRender()) {
		removeMeshChunk(chunk->getPos());
		return;
	}
	
	meshGenStream->queue(chunk->getPos(), priority);
	chunk->setDirty(false);
}

bool LocalDimension::adjacentExists(ivec3 pos, bool update, bool priority) {
	let chunk = getChunk(pos);
	if (!chunk) return false;
	if (update) meshChunk(chunk, priority, false);
	return true;
}