//
// Created by aurailus on 17/12/18.
//

#pragma once

#include "Camera.h"
#include "client/Window.h"

// Shaders
#include "shader/Shader.h"
#include "shader/SSAOShader.h"
#include "shader/BlurShader.h"
#include "shader/LightingShader.h"
#include "shader/WorldGeometryShader.h"
#include "shader/EntityGeometryShader.h"
#include "shader/GuiUniforms.h"

class Renderer {
public:
	Renderer();
	
	Renderer(glm::ivec2 win);
	
	void update(double delta);
	
	void beginChunkDeferredCalls();
	
	void beginEntityDeferredCalls();
	
	void endDeferredCalls();
	
	void beginGUIDrawCalls();
	
	void swapBuffers();
	
	void setShader(Shader& s);
	
	void setClearColor(unsigned char r, unsigned char g, unsigned char b);
	
	static void toggleDepthTest(bool enable);
	
	static void clearDepthBuffer();
	
	void setModelMatrix(const glm::mat4& modelMatrix);
	
	void setBones(std::vector<glm::mat4>& transforms);
	
	void setClipBounds(glm::vec4 bounds);
	
	void enableTexture(const Texture& texture);
	
	Window window;
	Camera camera;
	
private:
	void renderQuad();
	
	unsigned int quadVAO = 0;
	unsigned int quadVBO = 0;
	
	glm::vec4 clearColor{ 0, 0, 0, 1 };
//	Texture* activeTexture;
	
	WorldGeometryShader world;
	EntityGeometryShader entity;
	SSAOShader ssao;
	BlurShader blur;
	LightingShader light;
	
	Shader guiShader;
	GuiUniforms gu;
	
	Shader* currentShader;
	
	GLint currentModelUniform;
	double elapsedTime = 0;
	
	vec<CallbackRef> callbacks {};
};

