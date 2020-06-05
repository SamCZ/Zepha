//
// Created by aurailus on 26/11/18.
//

#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include "../../../util/Log.h"

void Shader::createFromString(std::string& vertexSource, std::string& fragmentSource, const std::string& geoSource) {
    compileShader(vertexSource, fragmentSource);
}

void Shader::createFromFile(const std::string& vertexFile, const std::string& fragmentFile, const std::string& geoFile) {
    compileShader(readFile(vertexFile), readFile(fragmentFile),
        geoFile == "" ? "" : readFile(geoFile));
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
    checkActive();
    glUniform1ui(loc, val);
}

void Shader::set(int loc, int val) {
    checkActive();
    glUniform1i(loc, val);
}

void Shader::set(int loc, float val) {
    checkActive();
    glUniform1f(loc, val);
}

void Shader::set(int loc, glm::vec3 val) {
    checkActive();
    glUniform3f(loc, val.x, val.y, val.z);
}

void Shader::set(int loc, glm::vec4 val) {
    checkActive();
    glUniform4f(loc, val.x, val.y, val.z, val.w);
}

void Shader::set(int loc, glm::mat4 val) {
    checkActive();
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
}

void Shader::setArr(int loc, unsigned int count, glm::mat4 &start) {
    checkActive();
    glUniformMatrix4fv(loc, count, GL_FALSE, glm::value_ptr(start));
}

void Shader::compileShader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geoSource) {
    shaderID = glCreateProgram();

    if (!shaderID) {
        std::cout << Log::err << "Error creating shader program." << Log::endl;
        exit(1);
    }

    addShader(shaderID, vertexSource, GL_VERTEX_SHADER);
    addShader(shaderID, fragmentSource, GL_FRAGMENT_SHADER);
    if (geoSource != "") addShader(shaderID, geoSource, GL_GEOMETRY_SHADER);

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
        std::string shaderTypeName = (shaderType == GL_VERTEX_SHADER) ? "vertex" : (shaderType == GL_FRAGMENT_SHADER) ? "fragment" : "geometry";

        glGetShaderInfoLog(shader, sizeof(eLog), nullptr, eLog);
        std::cout << Log::err << "Error compiling the " << shaderTypeName << " shader:\n" << eLog
            << Log::endl << Log::err << shaderCode << std::endl;
        return;
    }

    glAttachShader(program, shader);
}

Shader::~Shader() {
    cleanup();
}

void Shader::cleanup() {
    if (shaderID != 0) glDeleteProgram(shaderID);
}

void Shader::checkActive() {
    int cProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &cProgram);
    if (cProgram != shaderID) throw std::runtime_error("Attempted to set a uniform on an inactive shader!");
}