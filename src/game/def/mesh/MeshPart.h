//
// Created by aurailus on 02/12/18.
//

#pragma once

#include <vector>
#include <memory>

#include "ShaderMod.h"
#include "BlockModelVertex.h"

class AtlasRef;

struct MeshPart {
	MeshPart(const std::vector<BlockModelVertex>& vertices, const std::vector<unsigned int>& indices,
		std::shared_ptr<AtlasRef> texture, unsigned int blendInd, std::shared_ptr<AtlasRef> blendMask);
	
	std::vector<BlockModelVertex> vertices;
	std::vector<unsigned int> indices;
	
	std::shared_ptr<AtlasRef> texture;
	
	unsigned int blendInd = 0;
	std::shared_ptr<AtlasRef> blendMask;
	
	ShaderMod shaderMod = ShaderMod::NONE;
	float modValue = 0;
};