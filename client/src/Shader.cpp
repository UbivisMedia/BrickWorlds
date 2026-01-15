#include "Shader.h"
#include <iostream>
#include <vector>
#include <GL/glew.h>

Shader::Shader() : m_program(0) {}

Shader::~Shader() {
    if (m_program) {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}

unsigned int Shader::compileShader(unsigned int type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(static_cast<size_t>(len) + 1);
        glGetShaderInfoLog(shader, len, nullptr, log.data());

        std::cerr << "Shader compile failed: " << log.data() << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

bool Shader::linkProgram(unsigned int vertex, unsigned int fragment) {
    m_program = glCreateProgram();
    glAttachShader(m_program, vertex);
    glAttachShader(m_program, fragment);

    // Für #version 330 core nutzen wir layout(location=...) im Shader,
    // daher kein glBindAttribLocation nötig.

    glLinkProgram(m_program);

    GLint success = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint len = 0;
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(static_cast<size_t>(len) + 1);
        glGetProgramInfoLog(m_program, len, nullptr, log.data());
        std::cerr << "Program link failed: " << log.data() << std::endl;

        glDeleteProgram(m_program);
        m_program = 0;
        return false;
    }
    return true;
}

bool Shader::loadFromStrings(const char* vertexSrc, const char* fragmentSrc) {
    GLuint vertex = compileShader(GL_VERTEX_SHADER, vertexSrc);
    if (vertex == 0) return false;

    GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);
    if (fragment == 0) {
        glDeleteShader(vertex);
        return false;
    }

    bool ok = linkProgram(vertex, fragment);

    glDetachShader(m_program, vertex);
    glDetachShader(m_program, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return ok;
}

void Shader::use() const {
    glUseProgram(m_program);
}

void Shader::setMat4(const char* name, const float* value) const {
    GLint location = glGetUniformLocation(m_program, name);
    if (location >= 0) glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

void Shader::setVec3(const char* name, float x, float y, float z) const {
    GLint location = glGetUniformLocation(m_program, name);
    if (location >= 0) glUniform3f(location, x, y, z);
}

void Shader::setInt(const char* name, int value) const {
    GLint location = glGetUniformLocation(m_program, name);
    if (location >= 0) glUniform1i(location, value);
}
