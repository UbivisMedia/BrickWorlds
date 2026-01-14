#include "Shader.h"
#include <iostream>
#include <GL/glew.h>

Shader::Shader() : m_program(0) {
}

Shader::~Shader() {
    if (m_program) {
        glDeleteProgram(m_program);
    }
}

bool Shader::loadFromStrings(const char* vertexSrc, const char* fragmentSrc) {
    unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexSrc);
    if (vertex == 0) return false;

    unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);
    if (fragment == 0) {
        glDeleteShader(vertex);
        return false;
    }

    bool success = linkProgram(vertex, fragment);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return success;
}

unsigned int Shader::compileShader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
        return 0;
    }

    return shader;
}

bool Shader::linkProgram(unsigned int vertex, unsigned int fragment) {
    m_program = glCreateProgram();
    glAttachShader(m_program, vertex);
    glAttachShader(m_program, fragment);
    glLinkProgram(m_program);

    int success;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_program, 512, nullptr, infoLog);
        std::cerr << "Shader linking failed: " << infoLog << std::endl;
        return false;
    }

    return true;
}

void Shader::use() const {
    glUseProgram(m_program);
}

void Shader::setMat4(const char* name, const float* value) const {
    int location = glGetUniformLocation(m_program, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

void Shader::setVec3(const char* name, float x, float y, float z) const {
    int location = glGetUniformLocation(m_program, name);
    glUniform3f(location, x, y, z);
}

void Shader::setInt(const char* name, int value) const {
    int location = glGetUniformLocation(m_program, name);
    glUniform1i(location, value);
}
