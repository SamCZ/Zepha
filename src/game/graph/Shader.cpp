//
// Created by aurailus on 26/11/18.
//

#include "Shader.h"

Shader::Shader() {
    shaderID = 0;
}

void Shader::createFromString(std::string& vertexSource, std::string& fragmentSource) {
    compileShader(vertexSource, fragmentSource);
}

void Shader::createFromFile(const std::string& vertexFile, const std::string& fragmentFile) {
    std::string vertexSource = readFile(vertexFile);
    std::string fragmentSource = readFile(fragmentFile);

    compileShader(vertexSource, fragmentSource);
}

std::string Shader::readFile(const std::string& fileLocation) {
    std::string contents;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if (!fileStream.is_open()) {
        std::cout << Log::err << "Failed to open shader file '" << fileLocation << "'!" << Log::endl;
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

void Shader::compileShader(const std::string& vertexSource, const std::string& fragmentSource) {
    shaderID = glCreateProgram();

    if (!shaderID) {
        std::cout << Log::err << "Error creating the shader program." << Log::endl;
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
        std::cout << Log::err << "Error linking program: '" << eLog << "'." << Log::endl;
        return;
    }

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);

    if (!result) {
        glGetProgramInfoLog(shaderID, sizeof(eLog), nullptr, eLog);
        std::cout << Log::err << "Error validating program: '" << eLog << "'." << Log::endl;
        return;
    }
}

void Shader::use() {
    glUseProgram(shaderID);
}

GLint Shader::getUniform(const char *name) {
    return glGetUniformLocation(shaderID, name);
}

void Shader::cleanup() {
    if (shaderID != 0) {
        glDeleteProgram(shaderID);
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
        const char* shaderTypeString = (shaderType == GL_VERTEX_SHADER) ? "vertex" : "fragment";

        glGetShaderInfoLog(shader, sizeof(eLog), nullptr, eLog);
        std::cout << Log::err << "Error compiling the " << shaderTypeString << " shader: '" << eLog << "'";
        return;
    }

    glAttachShader(program, shader);
}

Shader::~Shader() {
    cleanup();
}