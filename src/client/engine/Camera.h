//
// Created by aurailus on 27/11/18.
//

#ifndef GLPROJECT_CAMERA_H
#define GLPROJECT_CAMERA_H

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera();
    void create(float buffWidth, float buffHeight, glm::vec3 up);

    void changeWindowDimensions(float buffWidth, float buffHeight);

    glm::vec3* getPosition();
    void setPosition(glm::vec3 pos);

    void setYaw(double yaw);
    void setPitch(double pitch);

    glm::mat4* getProjectionMatrix();
    glm::mat4* getOrthographicMatrix();

    glm::mat4 getViewMatrix();

    glm::vec3* getFront();
    glm::vec3* getRight();

    glm::vec2 getBufferDimensions();

    ~Camera();
private:
    glm::vec2 bufferDimensions;

    glm::mat4 projectionMatrix;
    glm::mat4 orthographicMatrix;

    glm::vec3 position;

    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    double yaw;
    double pitch;

    void createMatrices();
    void update();
};


#endif //GLPROJECT_CAMERA_H
