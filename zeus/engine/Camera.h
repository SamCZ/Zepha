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
    void create(glm::vec3 up);

    glm::vec3* getPosition();
    void setPosition(glm::vec3 pos);

    void setYaw(double yaw);
    void setPitch(double pitch);

    glm::mat4 calculateViewMatrix();

    ~Camera();

    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
private:
    glm::vec3 position;

    double yaw;
    double pitch;

    void update();
};


#endif //GLPROJECT_CAMERA_H
