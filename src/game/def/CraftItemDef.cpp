//
// Created by aurailus on 01/10/19.
//

#include "CraftItemDef.h"

#include "game/atlas/TextureAtlas.h"

CraftItemDef::CraftItemDef(const string& identifier, const string& name, u16 maxStackSize,
	const vec<string>& textures, const vec<AtlasTexture>& textureRefs) :
	CraftItemDef(identifier, 0, name, maxStackSize, textures, textureRefs) {}

CraftItemDef::CraftItemDef(const string& identifier, u16 index, const string& name,
	u16 maxStackSize, const vec<string>& textures, const vec<AtlasTexture>& textureRefs) :
	ItemDef(ItemDef::Type::CRAFTITEM, identifier, name, index, maxStackSize),
	textures(textures), textureRefs(textureRefs) {}

void CraftItemDef::createModel(TextureAtlas& atlas) {
	uptr<EntityMesh> mesh = make_unique<EntityMesh>();
	
	vec<EntityVertex> vertices;
	vec<u16> indices { 0, 1, 2, 2, 3, 0, 4, 7, 6, 6, 5, 4 };
	
	static const f32 xo = 0.040f;
	vec4 uv = textureRefs[0].getUVPos();
	
	for (u8 i = 0; i <= 1; i++) {
		f32 xx = xo * (i == 1 ? -1 : 1);
		vec<EntityVertex> myVerts = {
			{{ xx, -0.5, -0.5 }, { uv.x, uv.w, 0, 1 }, { 1, 1, 1 }, true, { (i == 1 ? -1 : 1), 0, 0 }, {}, {}},
			{{ xx,  0.5, -0.5 }, { uv.x, uv.y, 0, 1 }, { 1, 1, 1 }, true, { (i == 1 ? -1 : 1), 0, 0 }, {}, {}},
			{{ xx,  0.5,  0.5 }, { uv.z, uv.y, 0, 1 }, { 1, 1, 1 }, true, { (i == 1 ? -1 : 1), 0, 0 }, {}, {}},
			{{ xx, -0.5,  0.5 }, { uv.z, uv.w, 0, 1 }, { 1, 1, 1 }, true, { (i == 1 ? -1 : 1), 0, 0 }, {}, {}}
		};
		vertices.insert(vertices.end(), myVerts.begin(), myVerts.end());
	}
	
//	u16 indOffset = 8;
//	vec<u8> data = ref.getBytes();
//	for (u16 i = 0; i < 16 * 16; i++) {
//		vec2 samplePos = { i % 16, i / 16 };
//		vec2 off { samplePos.x / 16.f, samplePos.y / 16.f };
//		vec4 col = atlas.getPixel(ref, samplePos);
//
//		if (col.w < 0.5) continue;
//
//		if (samplePos.y == 0 || atlas.getPixel(ref, { samplePos.x, samplePos.y - 1 }).w < 0.5) {
//			std::vector<EntityVertex> myVerts = {
//				{{ -xo, 0.5 - off.y, -0.5 + off.x }, col, { 1, 1, 1 }, false, { 0, 1, 0 }, {}, {}},
//				{{ -xo, 0.5 - off.y, -0.5 + off.x + 0.0625 }, col, { 1, 1, 1 }, false, { 0, 1, 0 }, {}, {}},
//				{{ xo, 0.5 - off.y, -0.5 + off.x + 0.0625 }, col, { 1, 1, 1 }, false, { 0, 1, 0 }, {}, {}},
//				{{ xo, 0.5 - off.y, -0.5 + off.x }, col, { 1, 1, 1 }, false, { 0, 1, 0 }, {}, {}}};
//			vertices.insert(vertices.end(), myVerts.begin(), myVerts.end());
//			std::vector<unsigned int> myInds = { indOffset, indOffset + 1, indOffset + 2, indOffset + 2, indOffset + 3,
//				indOffset };
//			indices.insert(indices.end(), myInds.begin(), myInds.end());
//			indOffset += 4;
//		}
//
//		if (samplePos.y == 15 || atlas.getPixel(ref, { samplePos.x, samplePos.y + 1 }).w < 0.5) {
//			std::vector<EntityVertex> myVerts = {
//				{{ -xo, 0.5 - off.y - 0.0625, -0.5 + off.x }, col, { 1, 1, 1 }, false, { 0, -1, 0 }, {}, {}},
//				{{ -xo, 0.5 - off.y - 0.0625, -0.5 + off.x + 0.0625 }, col, { 1, 1, 1 }, false, { 0, -1, 0 }, {}, {}},
//				{{ xo, 0.5 - off.y - 0.0625, -0.5 + off.x + 0.0625 }, col, { 1, 1, 1 }, false, { 0, -1, 0 }, {}, {}},
//				{{ xo, 0.5 - off.y - 0.0625, -0.5 + off.x }, col, { 1, 1, 1 }, false, { 0, -1, 0 }, {}, {}}};
//			vertices.insert(vertices.end(), myVerts.begin(), myVerts.end());
//			std::vector<unsigned int> myInds = { indOffset, indOffset + 3, indOffset + 2, indOffset + 2, indOffset + 1,
//				indOffset };
//			indices.insert(indices.end(), myInds.begin(), myInds.end());
//			indOffset += 4;
//		}
//
//		if (samplePos.x == 0 || atlas.getPixel(ref, { samplePos.x - 1, samplePos.y }).w < 0.5) {
//			std::vector<EntityVertex> myVerts = {
//				{{ -xo, 0.5 - off.y - 0.0625, -0.5 + off.x }, col, { 1, 1, 1 }, false, { 0, 0, 1 }, {}, {}},
//				{{ -xo, 0.5 - off.y, -0.5 + off.x }, col, { 1, 1, 1 }, false, { 0, 0, 1 }, {}, {}},
//				{{ xo, 0.5 - off.y, -0.5 + off.x }, col, { 1, 1, 1 }, false, { 0, 0, 1 }, {}, {}},
//				{{ xo, 0.5 - off.y - 0.0625, -0.5 + off.x }, col, { 1, 1, 1 }, false, { 0, 0, 1 }, {}, {}}};
//			vertices.insert(vertices.end(), myVerts.begin(), myVerts.end());
//			std::vector<unsigned int> myInds = { indOffset, indOffset + 1, indOffset + 2, indOffset + 2, indOffset + 3,
//				indOffset };
//			indices.insert(indices.end(), myInds.begin(), myInds.end());
//			indOffset += 4;
//		}
//
//		if (samplePos.x == 15 || atlas.getPixel(ref, { samplePos.x + 1, samplePos.y }).w < 0.5) {
//			std::vector<EntityVertex> myVerts = {
//				{{ -xo, 0.5 - off.y - 0.0625, -0.5 + off.x + 0.0625 }, col, { 1, 1, 1 }, false, { 0, 0, -1 }, {}, {}},
//				{{ -xo, 0.5 - off.y, -0.5 + off.x + 0.0625 }, col, { 1, 1, 1 }, false, { 0, 0, -1 }, {}, {}},
//				{{ xo, 0.5 - off.y, -0.5 + off.x + 0.0625 }, col, { 1, 1, 1 }, false, { 0, 0, -1 }, {}, {}},
//				{{ xo, 0.5 - off.y - 0.0625, -0.5 + off.x + 0.0625 }, col, { 1, 1, 1 }, false, { 0, 0, -1 }, {}, {}}};
//			vertices.insert(vertices.end(), myVerts.begin(), myVerts.end());
//			std::vector<unsigned int> myInds = { indOffset, indOffset + 3, indOffset + 2, indOffset + 2, indOffset + 1,
//				indOffset };
//			indices.insert(indices.end(), myInds.begin(), myInds.end());
//			indOffset += 4;
//		}
//	}
//	mesh->create(vertices, indices);

	entityModel->fromMesh(std::move(mesh));
}

bool CraftItemDef::hasUse() {
	return callbacks.count(CraftItemDef::Callback::USE) || callbacks.count(CraftItemDef::Callback::USE_CLIENT);
}
