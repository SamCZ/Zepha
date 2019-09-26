//
// Created by aurailus on 27/11/18.
//

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "frustum/Frustum.h"

class Camera {
public:
    Camera();
    void create(float buffWidth, float buffHeight, glm::vec3 up);

    void changeWindowDimensions(glm::vec2 size);

    glm::vec3 getPos();
    void setPos(glm::vec3 pos);

    void setYaw(double yaw);
    void setPitch(double pitch);

    glm::mat4 getProjectionMatrix();
    glm::mat4 getOrthographicMatrix();

    glm::mat4 getViewMatrix();

    glm::vec3 getFront();
    glm::vec3 getRight();

    glm::vec2 getBufferDimensions();

    bool inFrustum(glm::vec3 &p);
    int inFrustum(FrustumAABB &b);

    ~Camera();
private:
    //Window Size
    glm::vec2 bufferDimensions;

    //Matrices
    glm::mat4 projectionMatrix;
    glm::mat4 orthographicMatrix;

    //Perspective Matrix Properties
    float fov = 70.0f;
    float ratio;
    float nearClip = 0.1f;
    float farClip = 1000.0f;

    //View Frustum
    Frustum frustum;

    //Movement translation variables
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    //Angle and Position
    glm::vec3 position;
    double yaw;
    double pitch;

    void createMatrices();
    void update();
};

