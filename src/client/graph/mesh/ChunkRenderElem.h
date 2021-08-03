#pragma once

#include "util/Types.h"

class Renderer;

/**
 * An abstract class for a visual representation of one or more chunks.
 * Currently used by MeshChunk, but in the future Mesh MapBlocks will use it as well.
 * Keeps track of the chunk positions that are using this element, and if it should be kept alive.
 */

struct ChunkRenderElem {
	ChunkRenderElem() = default;
	ChunkRenderElem(vec3 pos): pos(pos) {};
	
	/** Sets the element's visual position. */
	virtual void setPos(vec3 pos) {
		this->pos = pos;
	};
	
	/** Gets the element's visual position. */
	virtual vec3 getPos() {
		return pos;
	};
	
	/** Draws the element to the screen. */
	virtual void draw(Renderer& renderer) = 0;
	
	/**
	 * Specifies if a chunk is using this render element.
	 * Returns a boolean indicating if there are any chunks using the element.
	 */
	 
	virtual bool updateChunkUse(ivec3 chunk, bool used) = 0;
	
protected:

	/** The element's visual position. */
	vec3 pos {};
};
