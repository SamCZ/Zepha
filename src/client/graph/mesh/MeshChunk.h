#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "Mesh.h"
#include "util/Types.h"
#include "ChunkRenderElem.h"
#include "client/graph/Drawable.h"

/**
 * A drawable mesh of a single chunk.
 */

class MeshChunk : public ChunkRenderElem, Drawable {
public:
	
	/** A single vertex of a ChunkMesh. */
	struct Vertex {
		vec3   position;
		
		vec2   texCoords;
		u8vec3 blendColor;
		vec2   blendMaskCoords;
		
		f32    normal; // packed vec3
		u8vec4 light;
		
		u8     shaderMod;
		u8vec3 modValues;
	};
	
	/** Represents a MeshChunk's underlying mesh. */
	struct Mesh : public ::Mesh {
		Mesh(const Mesh&) = delete;
		Mesh(const vec<Vertex>& vertices, const vec<u32>& indices);
	};
	
	/** Creates a new MeshChunk with the data provided. */
	MeshChunk(const vec3 pos, const vec<Vertex>& vertices, const vec<u32>& indices);
	
	void draw(Renderer& renderer) override;
	
	bool updateChunkUse(ivec3 chunk, bool used) override;

private:
	
	/** The underlying mesh used by this chunk. */
	uptr<Mesh> mesh = nullptr;
};

#pragma clang diagnostic pop