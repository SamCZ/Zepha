//
// Created by aurailus on 26/11/18.
//

#pragma once

#include <GL/glew.h>

#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../../util/Log.h"

class Shader {
public:
    Shader();

    void createFromString(std::string& vertexSource, std::string& fragmentSource);
    void createFromFile(const std::string& vertexFile, const std::string& fragmentFile);

    virtual void postCreate() {};

    GLint get(const std::string &name);

    void use();
    static void clearShader();

    void set(int loc, uint val);
    void set(int loc, int val);
    void set(int loc, float val);
    void set(int loc, glm::vec3 val);
    void set(int loc, glm::mat4 val);

    void setArr(int loc, uint count, glm::mat4 &start);

    void cleanup();

    ~Shader();

private:
    std::string readFile(const std::string& fileLocation);

    void compileShader(const std::string& vertexSource, const std::string& fragmentSource);
    void addShader(GLuint program, const std::string& shaderCode, GLenum shaderType);

    void crashIfInactive();

    GLuint shaderID;
    std::string vertexFile = "string";
    std::string fragmentFile = "string";
};

