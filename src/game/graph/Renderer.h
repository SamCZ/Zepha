//
// Created by aurailus on 17/12/18.
//

#pragma once

#include "ChunkMesh.h"
#include "Shader.h"
#include "window/Window.h"
#include "Camera.h"
#include "Texture.h"
#include "GuiUniforms.h"
#include "uniform/WorldLightingUniforms.h"
#include "uniform/WorldGeometryUniforms.h"
#include "uniform/EntityGeometryUniforms.h"
#include "uniform/SSAOUniforms.h"
#include <glm/ext.hpp>
#include <noise/noise.h>

class Renderer {
public:
    Renderer();
    Renderer(GLint winWidth, GLint winHeight);

    void createWorldShaders();
    void createGUIShader();

    void update(double delta);

    void beginChunkDeferredCalls();
    void beginEntityDeferredCalls();
    void endDeferredCalls();
    void beginGUIDrawCalls();
    void swapBuffers();

    void setClearColor(unsigned char r, unsigned char g, unsigned char b);

    void setModelMatrix(const glm::mat4& modelMatrix);
    void setBones(std::vector<glm::mat4>& transforms);
    void enableTexture(Texture* texture);

    Window& getWindow();
    Camera& getCamera();

    ~Renderer();

    bool resized;
private:
    void renderQuad();

    unsigned int quadVAO = 0, quadVBO;
    float renderScale = 2.0f;

    Window window;
    Camera camera;

    unsigned int gBuffer, gPosition, gNormal, gColorSpec, rboDepth;
    unsigned int ssaoFBO, ssaoColorBuffer;
    unsigned int ssaoBlurFBO, ssaoColorBufferBlur;
    unsigned int sBuffer, sDepthMap;

    Texture swayTex;
    double swayOffset = 0;
    noise::module::Perlin swayNoise;
    unsigned char* swayData = nullptr;

    std::vector<glm::vec3> ssaoKernel;
    std::vector<glm::vec3> ssaoNoise;
    unsigned int ssaoTex;

    glm::vec4 clearColor {0, 0, 0, 1};
    Texture* activeTexture;

    Shader worldGeometryShader;
    WorldGeometryUniforms wgu;
    Shader entityGeometryShader;
    EntityGeometryUniforms egu;
    Shader ssaoShader;
    Shader ssaoBlur;
    SSAOUniforms sau;
    Shader worldLightingShader;
    WorldLightingUniforms wlu;
    Shader guiShader;
    GuiUniforms gu;

    GLint currentModelUniform;
    double elapsedTime = 0;
};

