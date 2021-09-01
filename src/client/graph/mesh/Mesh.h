#pragma once

#include "util/GL.h"
#include "util/Types.h"

/** Returns the stride and the offset of the member in the struct. */
#define STRIDE_OFFSET(Struct, Member) sizeof(struct Struct), (void*)offsetof(struct Struct, Member)

/** A renderable mesh. Inherited by other mesh types. */
class Mesh {
public:
	/** Draws the mesh to the screen. */
	virtual void draw() const;
	
	~Mesh();
	
protected:
	/** Generates the vertex and index arrays on the GPU. */
	void genArrays(usize vboLength, usize iboLength, const void* vertices, const void* indices);
	
	/** Creates a vertex attribute on the VBO. */
	void createVertexAttrib(u32 offset, u32 size, GLenum type, bool integral, u32 stride, const void* pointer);
	
	usize indCount = 0;
	u32 VAO = 0, VBO = 0, IBO = 0;
};
