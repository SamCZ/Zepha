//
// Created by aurailus on 26/11/18.
//

#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include "../../../util/Log.h"

void Shader::createFromString(std::string& vertexSource, std::string& fragmentSource) {
    compileShader(vertexSource, fragmentSource);
}

void Shader::createFromFile(const std::string& vertexFile, const std::string& fragmentFile) {
    this->vertexFile = vertexFile;
    this->fragmentFile = fragmentFile;

    std::string vertexSource = readFile(vertexFile);
    std::string fragmentSource = readFile(fragmentFile);

    compileShader(vertexSource, fragmentSource);

    postCreate();
}

std::string Shader::readFile(const std::string& fileLocation) {
    std::string contents;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if (!fileStream.is_open()) {
        std::cout << Log::err << "Failed to open shader file '" << fileLocation << "'." << Log::endl;
        exit(1);
    }

    std::string line;
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        contents.append(line + "\n");
    }

    fileStream.close();
    return contents;
}

void Shader::use() {
    glUseProgram(shaderID);
}

void Shader::clearShader() {
    glUseProgram(0);
}

GLint Shader::get(const std::string &name) {
    return glGetUniformLocation(shaderID, name.c_str());
}

void Shader::set(int loc, unsigned int val) {
    crashIfInactive();
    glUniform1ui(loc, val);
}

void Shader::set(int loc, int val) {
    crashIfInactive();
    glUniform1i(loc, val);
}

void Shader::set(int loc, float val) {
    crashIfInactive();
    glUniform1f(loc, val);
}

void Shader::set(int loc, glm::vec3 val) {
    crashIfInactive();
    glUniform3f(loc, val.x, val.y, val.z);
}

void Shader::set(int loc, glm::vec4 val) {
    crashIfInactive();
    glUniform4f(loc, val.x, val.y, val.z, val.w);
}

void Shader::set(int loc, glm::mat4 val) {
    crashIfInactive();
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
}

void Shader::setArr(int loc, unsigned int count, glm::mat4 &start) {
    crashIfInactive();
    glUniformMatrix4fv(loc, count, GL_FALSE, glm::value_ptr(start));
}

void Shader::compileShader(const std::string& vertexSource, const std::string& fragmentSource) {
    shaderID = glCreateProgram();

    if (!shaderID) {
        std::cout << Log::err << "Error creating shader program." << Log::endl;
        exit(1);
    }

    addShader(shaderID, vertexSource, GL_VERTEX_SHADER);
    addShader(shaderID, fragmentSource, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

    if (!result) {
        glGetProgramInfoLog(shaderID, sizeof(eLog), nullptr, eLog);
        std::cout << Log::err << "Error linking program.\n" << eLog << Log::endl;
        exit(1);
    }

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);

    if (!result) {
        glGetProgramInfoLog(shaderID, sizeof(eLog), nullptr, eLog);
        std::cout << Log::err << "Error validating program.\n" << eLog << Log::endl;
        exit(1);
    }
}

void Shader::addShader(GLuint program, const std::string& shaderCode, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);

    const GLchar* shaderCodeCStr = shaderCode.data();
    int shaderLength = static_cast<int>(shaderCode.length());

    glShaderSource(shader, 1, &shaderCodeCStr, &shaderLength);
    glCompileShader(shader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (!result) {
        std::string& shaderName = (shaderType == GL_VERTEX_SHADER) ? vertexFile : fragmentFile;

        glGetShaderInfoLog(shader, sizeof(eLog), nullptr, eLog);
        std::cout << Log::err << "-- Error compiling '" << shaderName << "' --\n" << eLog << Log::endl
                  << Log::err << shaderCode << std::endl;
        return;
    }

    glAttachShader(program, shader);
}

Shader::~Shader() {
    cleanup();
}

void Shader::cleanup() {
    if (shaderID != 0) {
        glDeleteProgram(shaderID);
    }
}

void Shader::crashIfInactive() {
    int cProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &cProgram);
    if (cProgram != shaderID) {
        std::cout << Log::err << "-- Attempted action on inactive shader! --" << Log::endl;
        throw std::exception();
    }
}