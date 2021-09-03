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

void LocalLuaEntity::setScale(f32 newScale) {
	if (appearance == "wireframe") model->fromMesh(
		WireframeEntity::createMesh({ {}, vec3(newScale) }, 0.035, wfStroke, wfFill));
	else DrawableEntity::setScale(newScale);
}

void LocalLuaEntity::setScale(vec3 newScale) {
	if (appearance == "wireframe") model->fromMesh(
		WireframeEntity::createMesh({ {}, newScale }, 0.035, wfStroke, wfFill));
	else DrawableEntity::setScale(newScale);
}

void LocalLuaEntity::setAppearance(const string& mode, const string& argA, const string& argB) {
	this->appearance = mode;
	
	if (mode == "gameobject" && (argA == "block" || argA == "craftitem")) {
		ItemDef& def = getGame()->getDefs().fromStr(argB);
		setModel(def.entityModel);
	}
	else if (mode == "model" && !argB.empty()) {
		auto model = std::make_shared<Model>();
		model->fromSerialized(getGame().l()->models.models[argA], { getGame().l()->textures[argB] });
		setModel(model);
	}
	else if (mode == "wireframe") {
		wfStroke = Util::hexToColorVec(argA);
		wfFill = argB.empty() ? vec4() : Util::hexToColorVec(argB);
		auto model = std::make_shared<Model>();
		model->fromMesh(WireframeEntity::createMesh({ {}, scale }, 0.035, wfStroke, wfFill));
		setModel(model);
	}
	else throw std::runtime_error("Invalid appearance arguments specified.");
}
