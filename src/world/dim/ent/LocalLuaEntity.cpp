//
// Created by aurailus on 2020-08-05.
//

#include "LocalLuaEntity.h"

#include "game/def/ItemDef.h"
#include "game/LocalSubgame.h"
#include "game/atlas/DefinitionAtlas.h"
#include "client/entity/WireframeEntity.h"

LocalLuaEntity::LocalLuaEntity(SubgamePtr game, DimensionPtr dim) :
	LuaEntity(game, dim), DrawableEntity(game, dim), Entity(game, dim) {}

void LocalLuaEntity::setAppearance(const std::string& dMode, const std::string& argA, const std::string& argB) {
	if (dMode == "gameobject" && (argA == "block" || argA == "craftitem")) {
		ItemDef& def = getGame()->getDefs().fromStr(argB);
		setModel(def.entityModel);
	}
	else if (dMode == "model" && !argB.empty()) {
		auto model = std::make_shared<Model>();
		model->fromSerialized(getGame().l()->models.models[argA], { getGame().l()->textures[argB] });
		setModel(model);
	}
	else throw std::runtime_error("Invalid appearance arguments specified.");
}