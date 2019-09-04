//
// Created by aurailus on 26/11/18.
//

#pragma once

#include <GL/glew.h>

#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include "../../util/Log.h"

class Shader {
public:
    Shader();

    void createFromString(std::string& vertexSource, std::string& fragmentSource);
    void createFromFile(const std::string& vertexFile, const std::string& fragmentFile);

    std::string readFile(const std::string& fileLocation);

    GLint getUniform(const char *name);

    void use();

    static void clearShader() {
        glUseProgram(0);
    }

    void cleanup();

    ~Shader();

private:
    GLuint shaderID;

    void compileShader(const std::string& vertexSource, const std::string& fragmentSource);
    void addShader(GLuint program, const std::string& shaderCode, GLenum shaderType);
};

