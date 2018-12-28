//
// Created by aurailus on 25/11/18.
//

#ifndef GLPROJECT_ENTITY_H
#define GLPROJECT_ENTITY_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "graphics/Mesh.h"
#include "graphics/Texture.h"

class Entity {
public:
    Entity();

    void create(Mesh* mesh);
    void create(Mesh* mesh, Texture* texture);
    void draw();
    void cleanup();

    void setTexture(Texture* texture);
    Texture* getTexture();

    void setPosition(glm::vec3 position);
    glm::vec3* getPosition();

    void setAngle(GLfloat angle);
    GLfloat* getAngle();

    void setScale(float scale);
    void setScale(glm::vec3 scale);
    glm::vec3* getScale();

    glm::mat4 getModelMatrix();

    ~Entity();
protected:
    Mesh* mesh;
    Texture* texture;

    glm::vec3 position;
    glm::vec3 scale;
    GLfloat angle;
};


#endif //GLPROJECT_ENTITY_H
