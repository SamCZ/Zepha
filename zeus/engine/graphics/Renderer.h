//
// Created by aurailus on 17/12/18.
//

#ifndef SRC_RENDERER_H
#define SRC_RENDERER_H

#include "Shader.h"
#include "../Window.h"
#include "../Camera.h"
#include "../Entity.h"
#include "../GuiEntity.h"
#include <ext.hpp>

class Renderer {
public:
    Renderer(GLint winWidth, GLint winHeight);

    void update();

    void begin();

    void enableWorldShader();
    void draw(Entity* entity);

    void enableGuiShader();
    void drawGui(GuiEntity* entity);

    void end();

    Window* getWindow();
    Camera* getCamera();

    ~Renderer();
private:
    Window* window;
    Camera* camera;

    glm::mat4 projectionMatrix;
    glm::mat4 orthographicMatrix;

    //World Shader
    Shader* worldShader;
    GLint uModel, uProj, uView, uOrtho, uGuiModel;

    Shader* guiShader;
};


#endif //SRC_RENDERER_H
