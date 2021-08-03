#include "World.h"

#include "util/Util.h"
#include "world/dim/Dimension.h"

//void World::update(f64 delta) {
//	for (auto& dimension : dimensions) dimension->update(delta);
//}

DimensionPtr World::getDefaultDimension() {
	if (defaultDimension.empty()) throw std::runtime_error("No default dimension was set.");
	return getDimension(defaultDimension);
}

void World::setDefaultDimension(const string& identifier) {
	defaultDimension = identifier;
}

DimensionPtr World::getDimension(u16 index) {
	if (dimensions.size() <= index) throw std::runtime_error(
		"Dimension #" + Util::toString(index) + " does not exist.");
	return dimensions[index];
}

DimensionPtr World::getDimension(const string& identifier) {
	if (dimensionIndexes.find(identifier) == dimensionIndexes.end() || dimensions.size() <= dimensionIndexes[identifier])
		throw std::runtime_error("Dimension '" + identifier + "'does not exist.");
	return dimensions[dimensionIndexes[identifier]];
}