//
// Created by aurailus on 23/07/19.
//

#pragma once

#include "util/Types.h"
#include "client/graph/mesh/MeshChunk.h"


struct MeshChunkDetails {
	ivec3 pos {};
	vec<u32> indices;
	vec<MeshChunk::Vertex> vertices;
};
