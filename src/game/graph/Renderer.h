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


    void update();

    void begin();
    void beginGUI();
    void setModelMatrix(glm::mat4& modelMatrix);
    void enableTexture(Texture* texture);
    void end();

    Window* getWindow();
    Camera* getCamera();

    ~Renderer();

    bool resized;
private:
    void enableWorldShader();
    void enableGuiShader();

    Window window;
    Camera camera;

    glm::vec4 clearColor {0.58f, 0.76f, 0.94f, 1.0f};
    Texture* activeTexture;

    Shader worldShader;
    WorldUniforms wu;

    Shader guiShader;
    GuiUniforms gu;

    bool mode; //false = world, true = gui
};


#endif //SRC_RENDERER_H
