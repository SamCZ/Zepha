#include "BlockModel.h"

#include "util/Vec.h"

BlockModel::BlockModel(vec<std::tuple<sptr<AtlasTexture>, optional<u32>, optional<sptr<AtlasTexture>>>> data):
	visible(true), culls(true) {
	
	textures.reserve(data.size());
	for (let& tuple : data) textures.emplace_back(get<0>(tuple));
	if (textures.size() == 0) throw std::runtime_error("Created cube with empty textures array.");
	
	/* Top Face */
	vec<BlockModelVertex> vertices = {
		{{ 0, 1, 0 }, {}, { 0, 0 }, { 0, 0 }},
		{{ 0, 1, 1 }, {}, { 0, 1 }, { 0, 1 }},
		{{ 1, 1, 1 }, {}, { 1, 1 }, { 1, 1 }},
		{{ 1, 1, 0 }, {}, { 1, 0 }, { 1, 0 }}};
	vec<u32> indices = { 0, 1, 2, 2, 3, 0 };
	
	u32 accessInd = (std::max)(0, (std::min)(static_cast<i32>(data.size() - 1), 0));
	parts[static_cast<i32>(EVec::TOP)].emplace_back(vertices, indices,
		get<0>(data[accessInd]), get<1>(data[accessInd]), get<2>(data[accessInd]));
	
	/* Bottom Face */
	vertices = {
		{{ 0, 0, 0 }, {}, { 0, 0 }, { 0, 0 }},
		{{ 1, 0, 0 }, {}, { 1, 0 }, { 1, 0 }},
		{{ 1, 0, 1 }, {}, { 1, 1 }, { 1, 1 }},
		{{ 0, 0, 1 }, {}, { 0, 1 }, { 0, 1 }}};
	indices = { 0, 1, 2, 2, 3, 0 };
	
	accessInd = (std::max)(0, (std::min)(static_cast<i32>(data.size() - 1), 1));
	parts[static_cast<i32>(EVec::BOTTOM)].emplace_back(vertices, indices,
		get<0>(data[accessInd]), get<1>(data[accessInd]), get<2>(data[accessInd]));
	
	/* Back Face */
	vertices = {
		{{ 0, 0, 0 }, {}, { 0, 1 }, { 0, 1 }},
		{{ 0, 0, 1 }, {}, { 1, 1 }, { 1, 1 }},
		{{ 0, 1, 1 }, {}, { 1, 0 }, { 1, 0 }},
		{{ 0, 1, 0 }, {}, { 0, 0 }, { 0, 0 }}};
	indices = { 0, 1, 2, 2, 3, 0 };
	
	accessInd = (std::max)(0, (std::min)(static_cast<i32>(data.size() - 1), 2));
	parts[static_cast<i32>(EVec::BACK)].emplace_back(vertices, indices,
		get<0>(data[accessInd]), get<1>(data[accessInd]), get<2>(data[accessInd]));
	
	/* Front Face */
	vertices = {
		{{ 1, 1, 1 }, {}, { 1, 0 }, { 1, 0 }},
		{{ 1, 0, 1 }, {}, { 1, 1 }, { 1, 1 }},
		{{ 1, 0, 0 }, {}, { 0, 1 }, { 0, 1 }},
		{{ 1, 1, 0 }, {}, { 0, 0 }, { 0, 0 }}};
	indices = { 0, 1, 2, 2, 3, 0 };
	
	accessInd = (std::max)(0, (std::min)(static_cast<i32>(data.size() - 1), 3));
	parts[static_cast<i32>(EVec::FRONT)].emplace_back(vertices, indices,
		get<0>(data[accessInd]), get<1>(data[accessInd]), get<2>(data[accessInd]));
	
	/* Left Face */
	vertices = {
		{{ 0, 0, 1 }, {}, { 0, 1 }, { 0, 1 }},
		{{ 1, 0, 1 }, {}, { 1, 1 }, { 1, 1 }},
		{{ 1, 1, 1 }, {}, { 1, 0 }, { 1, 0 }},
		{{ 0, 1, 1 }, {}, { 0, 0 }, { 0, 0 }}};
	indices = { 0, 1, 2, 2, 3, 0 };
	
	accessInd = (std::max)(0, (std::min)(static_cast<i32>(data.size() - 1), 4));
	parts[static_cast<i32>(EVec::LEFT)].emplace_back(vertices, indices,
		get<0>(data[accessInd]), get<1>(data[accessInd]), get<2>(data[accessInd]));
	
	/* Back Face */
	vertices = {
		{{ 0, 0, 0 }, {}, { 0, 1 }, { 0, 1 }},
		{{ 0, 1, 0 }, {}, { 0, 0 }, { 0, 0 }},
		{{ 1, 1, 0 }, {}, { 1, 0 }, { 1, 0 }},
		{{ 1, 0, 0 }, {}, { 1, 1 }, { 1, 1 }}};
	indices = { 0, 1, 2, 2, 3, 0 };
	
	accessInd = (std::max)(0, (std::min)(static_cast<i32>(data.size() - 1), 5));
	parts[static_cast<i32>(EVec::RIGHT)].emplace_back(vertices, indices,
		get<0>(data[accessInd]), get<1>(data[accessInd]), get<2>(data[accessInd]));
}
