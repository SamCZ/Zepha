#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/normal.hpp>

#include "MeshPart.h"

#include "game/atlas/asset/AtlasTexture.h"

MeshPart::MeshPart(const vec<BlockModelVertex>& vertices, const vec<u32>& indices,
	optional<sptr<AtlasTexture>> texture, optional<u32> blendInd, optional<sptr<AtlasTexture>> blendTexture) :
	
	vertices(vertices),
	indices(indices),
	texture(texture),
	blendInd(blendInd),
	blendTexture(blendTexture) {
	
	/*
	 * We assume these vertex structs do not have normals, and generate them here from the triangle information.
	 * To do this, we have to assume that each group of 3 indices is a triangle (which it would be hard for it to not be)
	 * and that no vertexes are shared on corners or places where vectors should be interpolated.
	 */
	
	/* Iterate through the indices to find all used vertices to add normals and adjust texture coordinates. */
	for (usize i = 0; i < this->indices.size() / 3; i++) {
		/* Get the three vertices. */
		let& p1 = this->vertices[this->indices[i * 3]];
		let& p2 = this->vertices[this->indices[i * 3 + 1]];
		let& p3 = this->vertices[this->indices[i * 3 + 2]];
		
		/* Get the normal of the formed triangle. */
		vec3 normal = glm::triangleNormal(p1.pos, p2.pos, p3.pos);
		
		/* Set the normal on the vertices. */
		p1.nml = normal;
		p2.nml = normal;
		p3.nml = normal;
	}
	
	/*
	 * If the MeshPart is being initialized on the client with an AtlasRef to base its values off of,
	 * it will set the UVs to the coordinates of the texture relative to the TextureAtlas.
	 */
	
	if (texture) {
		auto uv = (*texture)->getUVPos();
		
		/* Iterate through the vertices to adjust the texture coordinates to fit the textureAtlas. */
		for (BlockModelVertex& vertex: this->vertices) {
			
			/* Store the old positions in texUVs. */
			vertex.texUVs.x = vertex.tex.x;
			vertex.texUVs.y = vertex.tex.y;
			
			/* Generate solid coordinates for the defs positions. */
			vertex.tex.x = uv.x + ((uv.z - uv.x) * vertex.tex.x);
			vertex.tex.y = uv.y + ((uv.w - uv.y) * vertex.tex.y);
			
			if (blendTexture) {
				auto bUV = (*blendTexture)->getUVPos();
				
				/* Store the old positions in blendMaskUVs. */
				vertex.blendMaskUVs.x = vertex.blendMask.x;
				vertex.blendMaskUVs.y = vertex.blendMask.y;
				
				/* Generate solid coordinates for the defs positions. */
				vertex.blendMask.x = bUV.x + ((bUV.z - bUV.x) * vertex.blendMask.x);
				vertex.blendMask.y = bUV.y + ((bUV.w - bUV.y) * vertex.blendMask.y);
			}
			else {
				/* Negative One denotes full blend on fragment shader. */
				vertex.blendMask.x = -1;
				vertex.blendMask.y = -1;
			}
		}
	}
}
