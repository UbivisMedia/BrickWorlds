#pragma once

#include <string>

class Shader {
public:
    Shader();
    ~Shader();

    bool loadFromStrings(const char* vertexSrc, const char* fragmentSrc);
    void use() const;

    unsigned int getProgram() const { return m_program; }

    void setMat4(const char* name, const float* value) const;
    void setVec3(const char* name, float x, float y, float z) const;
    void setInt(const char* name, int value) const;

private:
    unsigned int m_program;

    unsigned int compileShader(unsigned int type, const char* source);
    bool linkProgram(unsigned int vertex, unsigned int fragment);
};
