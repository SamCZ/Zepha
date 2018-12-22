//
// Created by aurailus on 26/11/18.
//

#ifndef GLPROJECT_SHADER_H
#define GLPROJECT_SHADER_H

#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader {
public:
    Shader();

    void createFromString(const char* vertexSource, const char* fragmentSource);
    void createFromFile(const char* vertexFile, const char* fragmentFile);

    std::string readFile(const char* fileLocation);

    GLint getUniformLocation(const char* name);

    void useShader();

    static void clearShader() {
        glUseProgram(0);
    }

    void cleanup();

    ~Shader();

private:
    GLuint shaderID;

    void compileShader(const char* vertexSource, const char* fragmentSource);
    void addShader(GLuint program, const char* shaderCode, GLenum shaderType);
};


#endif //GLPROJECT_SHADER_H
