//
// Created by aurailus on 02/12/18.
//

#include "LocalDefinitionAtlas.h"

#include "game/def/BlockDef.h"
#include "TextureAtlas.h"

LocalDefinitionAtlas::LocalDefinitionAtlas(TextureAtlas& atlas) {
	//Invalid Node
	BlockModel invalidModel = BlockModel({{ atlas["_missing"], {}, {} }});
	BlockDef* invalid = new BlockDef();
	invalid->identifier = "invalid";
	invalid->name = "INVALID";
	invalid->maxStack = 64;
	invalid->model = invalidModel;
	invalid->farModel = invalidModel;
	invalid->sBoxes = {{{ 0, 0, 0 }, { 1, 1, 1 }}};
	invalid->cBoxes = {{{ 0, 0, 0 }, { 1, 1, 1 }}};
	defs.push_back(invalid);
	defTable.insert({ "invalid", 0 });
	
	//Air Node
	BlockModel nullModel {};
	BlockDef* air = new BlockDef();
	air->lightPropagates = true;
	air->identifier = "air";
	air->index = 1;
	air->name = "Air";
	air->maxStack = 64;
	air->model = nullModel;
	air->farModel = nullModel;
	air->solid = false;
	air->culls = false;
	defs.push_back(air);
	defTable.insert({ "air", 1 });
}

void LocalDefinitionAtlas::setIdentifiers(const std::vector<std::string>& identifiers) {
	for (unsigned int i = 0; i < identifiers.size(); i++) {
		if (i >= 2) defs.emplace_back(); //Don't emplace air or ignore
		defTable.insert({ identifiers[i], i });
	}
}

void LocalDefinitionAtlas::registerDef(ItemDef* def) {
	if (!defTable.count(def->identifier))
		throw std::runtime_error("Client/Server definition desync: " + def->identifier + ".");
	def->index = defTable[def->identifier];
	defs[def->index] = def;
}
