//
// Created by aurailus on 17/12/18.
//

#ifndef SRC_RENDERER_H
#define SRC_RENDERER_H

#include "ChunkMesh.h"
#include "Shader.h"
#include "window/Window.h"
#include "Camera.h"
#include "Texture.h"
#include "GuiUniforms.h"
#include "uniform/WorldLightingUniforms.h"
#include "uniform/WorldGeometryUniforms.h"
#include "uniform/EntityGeometryUniforms.h"
#include <ext.hpp>
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

    void setModelMatrix(const glm::mat4& modelMatrix);
    void enableTexture(Texture* texture);

    Window* getWindow();
    Camera* getCamera();

    ~Renderer();

    bool resized;
private:
    void renderQuad();

    unsigned int quadVAO = 0, quadVBO;
    float renderScale = 2.f;

    Window window;
    Camera camera;

    unsigned int gBuffer, gPosition, gNormal, gColorSpec, rboDepth;
    unsigned int sBuffer, sDepthMap;

    Texture* swayMap;
    double swayOffset = 0;
    noise::module::Perlin swayNoise;
    unsigned char* swayData = nullptr;

    glm::vec4 clearColor {0.58f, 0.76f, 0.94f, 1.0f};
    Texture* activeTexture;

    Shader worldGeometryShader;
    WorldGeometryUniforms wgu;
    Shader entityGeometryShader;
    EntityGeometryUniforms egu;
    Shader worldLightingShader;
    WorldLightingUniforms wlu;
    Shader guiShader;
    GuiUniforms gu;

    GLint currentModelUniform;
    double elapsedTime;
};


#endif //SRC_RENDERER_H
