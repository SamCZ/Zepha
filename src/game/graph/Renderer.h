//
// Created by aurailus on 17/12/18.
//

#ifndef SRC_RENDERER_H
#define SRC_RENDERER_H

#include "Mesh.h"
#include "Shader.h"
#include "window/Window.h"
#include "Camera.h"
#include "Texture.h"
#include "WorldUniforms.h"
#include "GuiUniforms.h"
#include <ext.hpp>

class Renderer {
public:
    Renderer();
    Renderer(GLint winWidth, GLint winHeight);

    void createWorldShader();
    void createGUIShader();

    void update();

    void beginWorldDrawCalls();
    void endWorldDrawCalls();

//    void beginGUI();
//    void end();

    void setModelMatrix(glm::mat4& modelMatrix);
    void enableTexture(Texture* texture);

    Window* getWindow();
    Camera* getCamera();

    ~Renderer();

    bool resized;
private:
//    void enableGuiShader();
    void renderQuad();

    unsigned int quadVAO = 0, quadVBO;

    Window window;
    Camera camera;

    unsigned int gBuffer;
    unsigned int gPosition, gNormal, gColorSpec;

//    glm::vec4 clearColor {0.58f, 0.76f, 0.94f, 1.0f};
    glm::vec4 clearColor {0.0f, 0.0f, 0.0f, 1.0f};
    Texture* activeTexture;

    Shader worldGeometryShader;
    Shader worldLightingShader;
    WorldUniforms wu;

    Shader guiShader;
    GuiUniforms gu;

    bool mode; //false = world, true = gui
};


#endif //SRC_RENDERER_H
