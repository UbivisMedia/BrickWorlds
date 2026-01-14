#include "Shader.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#elif defined(__linux__)
#include <GL/gl.h>
#endif

// OpenGL 2.0+ function pointers
#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS 0x8B81
#endif
#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS 0x8B82
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER 0x8B30
#endif
#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER 0x8B31
#endif

// Only declare if not already defined by GL headers
#ifndef APIENTRY
#define APIENTRY
#endif

// Use existing types from glext.h if available, otherwise define our own
#ifndef PFNGLCREATESHADERPROC
typedef unsigned int (APIENTRY* PFNGLCREATESHADERPROC)(unsigned int);
#endif
#ifndef PFNGLSHADERSOURCEPROC
typedef void (APIENTRY* PFNGLSHADERSOURCEPROC)(unsigned int, int, const char**, const int*);
#endif
#ifndef PFNGLCOMPILESHADERPROC
typedef void (APIENTRY* PFNGLCOMPILESHADERPROC)(unsigned int);
#endif
#ifndef PFNGLGETSHADERIVPROC
typedef void (APIENTRY* PFNGLGETSHADERIVPROC)(unsigned int, unsigned int, int*);
#endif
#ifndef PFNGLGETSHADERINFOLOGPROC
typedef void (APIENTRY* PFNGLGETSHADERINFOLOGPROC)(unsigned int, int, int*, char*);
#endif
#ifndef PFNGLCREATEPROGRAMPROC
typedef unsigned int (APIENTRY* PFNGLCREATEPROGRAMPROC)(void);
#endif
#ifndef PFNGLATTACHSHADERPROC
typedef void (APIENTRY* PFNGLATTACHSHADERPROC)(unsigned int, unsigned int);
#endif
#ifndef PFNGLLINKPROGRAMPROC
typedef void (APIENTRY* PFNGLLINKPROGRAMPROC)(unsigned int);
#endif
#ifndef PFNGLGETPROGRAMIVPROC
typedef void (APIENTRY* PFNGLGETPROGRAMIVPROC)(unsigned int, unsigned int, int*);
#endif
#ifndef PFNGLGETPROGRAMINFOLOGPROC
typedef void (APIENTRY* PFNGLGETPROGRAMINFOLOGPROC)(unsigned int, int, int*, char*);
#endif
#ifndef PFNGLUSEPROGRAMPROC
typedef void (APIENTRY* PFNGLUSEPROGRAMPROC)(unsigned int);
#endif
#ifndef PFNGLDELETESHADERPROC
typedef void (APIENTRY* PFNGLDELETESHADERPROC)(unsigned int);
#endif
#ifndef PFNGLGETUNIFORMLOCATIONPROC
typedef int (APIENTRY* PFNGLGETUNIFORMLOCATIONPROC)(unsigned int, const char*);
#endif
#ifndef PFNGLUNIFORMMATRIX4FVPROC
typedef void (APIENTRY* PFNGLUNIFORMMATRIX4FVPROC)(int, int, unsigned char, const float*);
#endif
#ifndef PFNGLUNIFORM3FPROC
typedef void (APIENTRY* PFNGLUNIFORM3FPROC)(int, float, float, float);
#endif
#ifndef PFNGLUNIFORM1IPROC
typedef void (APIENTRY* PFNGLUNIFORM1IPROC)(int, int);
#endif


// Global function pointers
static PFNGLCREATESHADERPROC glCreateShader = nullptr;
static PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
static PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
static PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
static PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
static PFNGLATTACHSHADERPROC glAttachShader = nullptr;
static PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
static PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
static PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
static PFNGLDELETESHADERPROC glDeleteShader = nullptr;
static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
static PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
static PFNGLUNIFORM3FPROC glUniform3f = nullptr;
static PFNGLUNIFORM1IPROC glUniform1i = nullptr;

// Load OpenGL functions
static bool loadGLFunctions() {
    static bool loaded = false;
    if (loaded) return true;

#ifdef _WIN32
    HMODULE gl = LoadLibraryA("opengl32.dll");
    auto wglGetProcAddress = (void* (*)(const char*))GetProcAddress(gl, "wglGetProcAddress");

    glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
    glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
    glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
    glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
    glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
#elif defined(__linux__)
    // On Linux, these are typically available directly
    glCreateShader = (PFNGLCREATESHADERPROC)glXGetProcAddress((const GLubyte*)"glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)glXGetProcAddress((const GLubyte*)"glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)glXGetProcAddress((const GLubyte*)"glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)glXGetProcAddress((const GLubyte*)"glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glXGetProcAddress((const GLubyte*)"glGetShaderInfoLog");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)glXGetProcAddress((const GLubyte*)"glCreateProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC)glXGetProcAddress((const GLubyte*)"glAttachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)glXGetProcAddress((const GLubyte*)"glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)glXGetProcAddress((const GLubyte*)"glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glXGetProcAddress((const GLubyte*)"glGetProgramInfoLog");
    glUseProgram = (PFNGLUSEPROGRAMPROC)glXGetProcAddress((const GLubyte*)"glUseProgram");
    glDeleteShader = (PFNGLDELETESHADERPROC)glXGetProcAddress((const GLubyte*)"glDeleteShader");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glXGetProcAddress((const GLubyte*)"glGetUniformLocation");
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glXGetProcAddress((const GLubyte*)"glUniformMatrix4fv");
    glUniform3f = (PFNGLUNIFORM3FPROC)glXGetProcAddress((const GLubyte*)"glUniform3f");
    glUniform1i = (PFNGLUNIFORM1IPROC)glXGetProcAddress((const GLubyte*)"glUniform1i");
#endif

    loaded = true;
    return glCreateShader != nullptr;
}

Shader::Shader() : m_program(0) {
    loadGLFunctions();
}

Shader::~Shader() {
    // Note: Would need glDeleteProgram function pointer
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
    glUniformMatrix4fv(location, 1, 0, value);
}

void Shader::setVec3(const char* name, float x, float y, float z) const {
    int location = glGetUniformLocation(m_program, name);
    glUniform3f(location, x, y, z);
}

void Shader::setInt(const char* name, int value) const {
    int location = glGetUniformLocation(m_program, name);
    glUniform1i(location, value);
}

