//
// Created by aurailus on 01/10/19.
//

#include "CraftItemDef.h"

#include "game/atlas/TextureAtlas.h"
#include "game/atlas/asset/AtlasRef.h"

CraftItemDef::CraftItemDef(const std::string& identifier, const std::string& name, unsigned short maxStackSize,
	const std::vector<std::string>& textures, const std::vector<std::shared_ptr<AtlasRef>>& textureRefs) :
	CraftItemDef(identifier, 0, name, maxStackSize, textures, textureRefs) {}

CraftItemDef::CraftItemDef(const std::string& identifier, unsigned int index, const std::string& name,
	unsigned short maxStackSize, const std::vector<std::string>& textures,
	const std::vector<std::shared_ptr<AtlasRef>>& textureRefs) :
	
	ItemDef(ItemDef::Type::CRAFTITEM, identifier, name, index, maxStackSize),
	textures(textures),
	textureRefs(textureRefs) {}

void CraftItemDef::createModel(TextureAtlas& atlas) {
	std::unique_ptr<EntityMesh> mesh = std::make_unique<EntityMesh>();
	
	std::vector<EntityVertex> vertices;
	std::vector<unsigned int> indices{ 0, 1, 2, 2, 3, 0, 4, 7, 6, 6, 5, 4 };
	
	const std::shared_ptr<AtlasRef>& ref = textureRefs[0];
	static const float xo = 0.040f;
	
	for (unsigned int i = 0; i <= 1; i++) {
		float xx = xo * (i == 1 ? -1 : 1);
		std::vector<EntityVertex> myVerts = {
			{{ xx, -0.5, -0.5 }, { ref->uv.x, ref->uv.w, 0, 1 }, { 1, 1, 1 }, true, { (i == 1 ? -1 : 1), 0, 0 }, {},
				{}},
			{{ xx, 0.5, -0.5 }, { ref->uv.x, ref->uv.y, 0, 1 }, { 1, 1, 1 }, true, { (i == 1 ? -1 : 1), 0, 0 }, {}, {}},
			{{ xx, 0.5, 0.5 }, { ref->uv.z, ref->uv.y, 0, 1 }, { 1, 1, 1 }, true, { (i == 1 ? -1 : 1), 0, 0 }, {}, {}},
			{{ xx, -0.5, 0.5 }, { ref->uv.z, ref->uv.w, 0, 1 }, { 1, 1, 1 }, true, { (i == 1 ? -1 : 1), 0, 0 }, {}, {}}
		};
		vertices.insert(vertices.end(), myVerts.begin(), myVerts.end());
	}
	
	unsigned int indOffset = 8;
	for (unsigned int i = 0; i < 16 * 16; i++) {
		glm::vec2 samplePos = { i % 16, i / 16 };
		glm::vec2 off{ samplePos.x / 16.f, samplePos.y / 16.f };
		glm::vec4 col = atlas.getPixel(ref, samplePos);
		
		if (col.w < 0.5) continue;
		
		if (samplePos.y == 0 || atlas.getPixel(ref, { samplePos.x, samplePos.y - 1 }).w < 0.5) {
			std::vector<EntityVertex> myVerts = {
				{{ -xo, 0.5 - off.y, -0.5 + off.x }, col, { 1, 1, 1 }, false, { 0, 1, 0 }, {}, {}},
				{{ -xo, 0.5 - off.y, -0.5 + off.x + 0.0625 }, col, { 1, 1, 1 }, false, { 0, 1, 0 }, {}, {}},
				{{ xo, 0.5 - off.y, -0.5 + off.x + 0.0625 }, col, { 1, 1, 1 }, false, { 0, 1, 0 }, {}, {}},
				{{ xo, 0.5 - off.y, -0.5 + off.x }, col, { 1, 1, 1 }, false, { 0, 1, 0 }, {}, {}}};
			vertices.insert(vertices.end(), myVerts.begin(), myVerts.end());
			std::vector<unsigned int> myInds = { indOffset, indOffset + 1, indOffset + 2, indOffset + 2, indOffset + 3,
				indOffset };
			indices.insert(indices.end(), myInds.begin(), myInds.end());
			indOffset += 4;
		}
		
		if (samplePos.y == 15 || atlas.getPixel(ref, { samplePos.x, samplePos.y + 1 }).w < 0.5) {
			std::vector<EntityVertex> myVerts = {
				{{ -xo, 0.5 - off.y - 0.0625, -0.5 + off.x }, col, { 1, 1, 1 }, false, { 0, -1, 0 }, {}, {}},
				{{ -xo, 0.5 - off.y - 0.0625, -0.5 + off.x + 0.0625 }, col, { 1, 1, 1 }, false, { 0, -1, 0 }, {}, {}},
				{{ xo, 0.5 - off.y - 0.0625, -0.5 + off.x + 0.0625 }, col, { 1, 1, 1 }, false, { 0, -1, 0 }, {}, {}},
				{{ xo, 0.5 - off.y - 0.0625, -0.5 + off.x }, col, { 1, 1, 1 }, false, { 0, -1, 0 }, {}, {}}};
			vertices.insert(vertices.end(), myVerts.begin(), myVerts.end());
			std::vector<unsigned int> myInds = { indOffset, indOffset + 3, indOffset + 2, indOffset + 2, indOffset + 1,
				indOffset };
			indices.insert(indices.end(), myInds.begin(), myInds.end());
			indOffset += 4;
		}
		
		if (samplePos.x == 0 || atlas.getPixel(ref, { samplePos.x - 1, samplePos.y }).w < 0.5) {
			std::vector<EntityVertex> myVerts = {
				{{ -xo, 0.5 - off.y - 0.0625, -0.5 + off.x }, col, { 1, 1, 1 }, false, { 0, 0, 1 }, {}, {}},
				{{ -xo, 0.5 - off.y, -0.5 + off.x }, col, { 1, 1, 1 }, false, { 0, 0, 1 }, {}, {}},
				{{ xo, 0.5 - off.y, -0.5 + off.x }, col, { 1, 1, 1 }, false, { 0, 0, 1 }, {}, {}},
				{{ xo, 0.5 - off.y - 0.0625, -0.5 + off.x }, col, { 1, 1, 1 }, false, { 0, 0, 1 }, {}, {}}};
			vertices.insert(vertices.end(), myVerts.begin(), myVerts.end());
			std::vector<unsigned int> myInds = { indOffset, indOffset + 1, indOffset + 2, indOffset + 2, indOffset + 3,
				indOffset };
			indices.insert(indices.end(), myInds.begin(), myInds.end());
			indOffset += 4;
		}
		
		if (samplePos.x == 15 || atlas.getPixel(ref, { samplePos.x + 1, samplePos.y }).w < 0.5) {
			std::vector<EntityVertex> myVerts = {
				{{ -xo, 0.5 - off.y - 0.0625, -0.5 + off.x + 0.0625 }, col, { 1, 1, 1 }, false, { 0, 0, -1 }, {}, {}},
				{{ -xo, 0.5 - off.y, -0.5 + off.x + 0.0625 }, col, { 1, 1, 1 }, false, { 0, 0, -1 }, {}, {}},
				{{ xo, 0.5 - off.y, -0.5 + off.x + 0.0625 }, col, { 1, 1, 1 }, false, { 0, 0, -1 }, {}, {}},
				{{ xo, 0.5 - off.y - 0.0625, -0.5 + off.x + 0.0625 }, col, { 1, 1, 1 }, false, { 0, 0, -1 }, {}, {}}};
			vertices.insert(vertices.end(), myVerts.begin(), myVerts.end());
			std::vector<unsigned int> myInds = { indOffset, indOffset + 3, indOffset + 2, indOffset + 2, indOffset + 1,
				indOffset };
			indices.insert(indices.end(), myInds.begin(), myInds.end());
			indOffset += 4;
		}
	}
	
	mesh->create(vertices, indices);
	entityModel->fromMesh(std::move(mesh));
}

bool CraftItemDef::hasUse() {
	return callbacks.count(CraftItemDef::Callback::USE) || callbacks.count(CraftItemDef::Callback::USE_CLIENT);
}
