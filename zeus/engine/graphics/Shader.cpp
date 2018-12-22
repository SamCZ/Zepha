//
// Created by aurailus on 26/11/18.
//

#include <cstring>
#include "Shader.h"

Shader::Shader() {
    shaderID = 0;
}

void Shader::createFromString(const char *vertexSource, const char *fragmentSource) {
    compileShader(vertexSource, fragmentSource);
}

void Shader::createFromFile(const char *vertexFile, const char *fragmentFile) {
    std::string vertexSource = readFile(vertexFile);
    std::string fragmentSource = readFile(fragmentFile);

    compileShader(vertexSource.c_str(), fragmentSource.c_str());
}

std::string Shader::readFile(const char* fileLocation) {
    std::string contents;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if (!fileStream.is_open()) {
        printf("Failed to open shader file! %s", fileLocation);
        return "";
    }

    std::string line;

    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        contents.append(line + "\n");
    }

    fileStream.close();
    return contents;
}

void Shader::compileShader(const char *vertexSource, const char *fragmentSource) {
    shaderID = glCreateProgram();

    if (!shaderID) {
        printf("Error creating the shader program.");
        return;
    }

    addShader(shaderID, vertexSource, GL_VERTEX_SHADER);
    addShader(shaderID, fragmentSource, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

    if (!result) {
        glGetProgramInfoLog(shaderID, sizeof(eLog), nullptr, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);

    if (!result) {
        glGetProgramInfoLog(shaderID, sizeof(eLog), nullptr, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }
}

void Shader::useShader() {
    glUseProgram(shaderID);
}

GLint Shader::getUniformLocation(const char* name) {
    return glGetUniformLocation(shaderID, name);
}

void Shader::cleanup() {
    if (shaderID != 0) {
        glDeleteProgram(shaderID);
    }
}

void Shader::addShader(GLuint program, const char *shaderCode, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    const GLchar* code;
    code = shaderCode;

    GLint codeLength;
    codeLength = static_cast<GLint>(strlen(shaderCode));

    glShaderSource(shader, 1, &code, &codeLength);
    glCompileShader(shader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (!result) {
        const char* shaderTypeString = (shaderType == GL_VERTEX_SHADER) ? "vertex" : "fragment";

        glGetShaderInfoLog(shader, sizeof(eLog), nullptr, eLog);
        printf("Error compiling the %s shader:\n'%s'\n", shaderTypeString, eLog);
        return;
    }

    glAttachShader(program, shader);
}

Shader::~Shader() {
    cleanup();
}