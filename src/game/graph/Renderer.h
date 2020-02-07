//
// Created by aurailus on 17/12/18.
//

#pragma once

#include <glm/gtx/transform.hpp>
#include <noise/noise.h>
#include "Camera.h"
#include "Texture.h"
#include "window/Window.h"
#include "shader/Shader.h"
#include "shader/SSAOShader.h"
#include "shader/BlurShader.h"
#include "shader/LightingShader.h"
#include "shader/WorldGeometryShader.h"
#include "shader/EntityGeometryShader.h"
#include "shader/GuiUniforms.h"
#include "meshtypes/ChunkMesh.h"

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

    void setClearColor(unsigned char r, unsigned char g, unsigned char b);
    void toggleDepthTest(bool enable);
    void clearDepthBuffer();

    void setModelMatrix(const glm::mat4& modelMatrix);
    void setBones(std::vector<glm::mat4>& transforms);
    void enableTexture(Texture* texture);

    Window window;
    Camera camera;
private:
    void renderQuad();

    unsigned int quadVAO = 0, quadVBO;

    unsigned int sBuffer, sDepthMap;

    glm::vec4 clearColor {0, 0, 0, 1};
    Texture* activeTexture;

    WorldGeometryShader world;
    EntityGeometryShader entity;
    SSAOShader ssao;
    BlurShader blur;
    LightingShader lighting;

    Shader guiShader;
    GuiUniforms gu;

    GLint currentModelUniform;
    double elapsedTime = 0;
};

