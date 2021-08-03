#pragma once

#include <unordered_set>

#include "util/Vec.h"
#include "util/Types.h"
#include "util/CovariantPtr.h"

class Subgame;
class Dimension;

/**
 * Manages all loaded dimensions.
 * Also handles all inventories, and sending mod messages.
 * LocalWorld and ServerWorld are children of this class.
 */

class World {
public:
	
	World(const World& o) = delete;
	
	explicit World(SubgamePtr game) : game(game) {};
	
	/** Creates a new dimension with the identifier and biomes provided. */
	virtual DimensionPtr createDimension(const string& identifier, std::unordered_set<string>& biomes) = 0;
	
	/** Gets the default dimension, throws if no default is set. */
	virtual DimensionPtr getDefaultDimension();
	
	/** Sets the identifier of the default dimension. */
	virtual void setDefaultDimension(const string& defaultDimension);
	
	/** Gets a dimension by its index, throws if there is none. */
	virtual DimensionPtr getDimension(u16 index);
	
	/** Gets a dimension by its identifier, throws if there is none. */
	virtual DimensionPtr getDimension(const string& identifier);
	
	/** Sends a mod message on the channel specified. */
	virtual void sendMessage(const string& channel, const string& message) = 0;
	
	/** Returns a reference to the world's inventory refs. */
	virtual InventoryRefsPtr getRefs() = 0;
	
protected:
	
	/** The identifier of the default dimension. */
	string defaultDimension {};
	
	/** A vector of dimensions in the world. */
	vec<sptr<Dimension>> dimensions;
	
	/** A map of dimension identifiers to indexes. */
	std::unordered_map<string, u16> dimensionIndexes;
	
	/** A reference to the subgame. */
	SubgamePtr game;
};
