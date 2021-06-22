//
// DimensionBase subclass that handles block light and sunlight.
// This class was split from DimensionBase to help differentiate
// lighting functions from basic world manipulation.
// Created by aurailus on 2020-05-26.
//

#pragma once

#include <queue>
#include <unordered_set>

#include "DimensionBase.h"

#include "util/Types.h"

class Target;
class Player;

class Dimension : public DimensionBase {
public:
	typedef std::unordered_set<ivec3, Vec::ivec3> relitChunks;
	
	Dimension(const Dimension& o) = delete;
	
	Dimension(SubgamePtr game, World& world, const string& identifier, u16 ind, sptr<MapGen> mapGen) :
		DimensionBase(game, world, identifier, ind, std::move(mapGen)) {}
	
	// Override setBlock to update lighting.
	bool setBlock(ivec3 pos, u16 block) override;
	
	virtual f64 blockHit(const Target& target, PlayerPtr player) = 0;
	
	virtual void blockPlace(const Target& target, PlayerPtr player) = 0;
	
	virtual void blockPlaceOrInteract(const Target& target, PlayerPtr player) = 0;
	
	virtual void blockInteract(const Target& target, PlayerPtr player) = 0;
	
	virtual void wieldItemUse(const Target& target, PlayerPtr player) = 0;
	
	virtual i64 nextEntityInd() = 0;
	
	// Calculate light propogation around MapBlock edges,
	// Called after a new mapblock is inserted into the dimension.
//    relitChunks calculateEdgeLight(glm::ivec3 mbPos);
protected:
	
	// Lighting propagation.
	
	virtual relitChunks propogateAddNodes();
	
	virtual relitChunks propogateRemoveNodes();
	
private:
	
	// Other Lighting methods.
	
	static inline bool containsWorldPos(Chunk* chunk, glm::ivec3 pos);
	
	inline u8vec4 getLight(ivec3 worldPos, Chunk* chunk = nullptr);
	
	void calculateHorizontalEdge(sptr<Chunk> a, sptr<Chunk> b);
	
	void calculateVerticalEdge(sptr<Chunk> above, sptr<Chunk> below);
	
	inline void addBlockLight(ivec3 pos, ivec3 light);
	
	inline void removeBlockLight(ivec3 pos);
	
	inline void reflowLight(ivec3 pos);
	
	inline void removeSunlight(ivec3 pos);
	
	inline void setAndReflowSunlight(ivec3 pos, u8 level);
	
	struct LightAddNode {
		LightAddNode(u16 index, Chunk* chunk) : index(index), chunk(chunk) {};
		u16 index;
		Chunk* chunk;
	};
	
	struct LightRemoveNode {
		LightRemoveNode(u16 index, u16 value, Chunk* chunk) : index(index), value(value), chunk(chunk) {};
		u16 index;
		u16 value;
		Chunk* chunk;
	};
	
	static constexpr u8 SUNLIGHT_CHANNEL = 3;
	array<std::queue<LightAddNode>, 4> lightAddQueue;
	array<std::queue<LightRemoveNode>, 4> lightRemoveQueue;
};
