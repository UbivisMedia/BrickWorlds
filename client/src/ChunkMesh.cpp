#include "ChunkMesh.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#elif defined(__linux__)
#include <GL/gl.h>
#include <GL/glx.h>
#endif

// OpenGL function pointers
#ifndef APIENTRY
#define APIENTRY
#endif

#ifndef PFNGLGENVERTEXARRAYSPROC
typedef void (APIENTRY* PFNGLGENVERTEXARRAYSPROC)(int, unsigned int*);
#endif
#ifndef PFNGLBINDVERTEXARRAYPROC
typedef void (APIENTRY* PFNGLBINDVERTEXARRAYPROC)(unsigned int);
#endif
#ifndef PFNGLGENBUFFERSPROC
typedef void (APIENTRY* PFNGLGENBUFFERSPROC)(int, unsigned int*);
#endif
#ifndef PFNGLBINDBUFFERPROC
typedef void (APIENTRY* PFNGLBINDBUFFERPROC)(unsigned int, unsigned int);
#endif
#ifndef PFNGLBUFFERDATAPROC
typedef void (APIENTRY* PFNGLBUFFERDATAPROC)(unsigned int, ptrdiff_t, const void*, unsigned int);
#endif
#ifndef PFNGLENABLEVERTEXATTRIBARRAYPROC
typedef void (APIENTRY* PFNGLENABLEVERTEXATTRIBARRAYPROC)(unsigned int);
#endif
#ifndef PFNGLVERTEXATTRIBPOINTERPROC
typedef void (APIENTRY* PFNGLVERTEXATTRIBPOINTERPROC)(unsigned int, int, unsigned int, unsigned char, int, const void*);
#endif

#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 0x8892
#endif
#ifndef GL_STATIC_DRAW
#define GL_STATIC_DRAW 0x88E4
#endif
#ifndef GL_FLOAT
#define GL_FLOAT 0x1406
#endif


static PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
static PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
static PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
static PFNGLBUFFERDATAPROC glBufferData = nullptr;
static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;

static bool loadMeshFunctions() {
    static bool loaded = false;
    if (loaded) return true;

#ifdef _WIN32
    HMODULE gl = LoadLibraryA("opengl32.dll");
    auto wglGetProcAddress = (void* (*)(const char*))GetProcAddress(gl, "wglGetProcAddress");

    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
    glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
#elif defined(__linux__)
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glXGetProcAddress((const GLubyte*)"glGenVertexArrays");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glXGetProcAddress((const GLubyte*)"glBindVertexArray");
    glGenBuffers = (PFNGLGENBUFFERSPROC)glXGetProcAddress((const GLubyte*)"glGenBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC)glXGetProcAddress((const GLubyte*)"glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)glXGetProcAddress((const GLubyte*)"glBufferData");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress((const GLubyte*)"glEnableVertexAttribArray");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glXGetProcAddress((const GLubyte*)"glVertexAttribPointer");
#endif

    loaded = true;
    return glGenVertexArrays != nullptr;
}

ChunkMesh::ChunkMesh() : m_vao(0), m_vbo(0), m_vertexCount(0) {
    loadMeshFunctions();
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
}

ChunkMesh::~ChunkMesh() {
    // Cleanup would need glDeleteBuffers/glDeleteVertexArrays
}

void ChunkMesh::getBlockColor(BrickWorlds::Core::BlockID id, float& r, float& g, float& b) {
    switch (id) {
    case BrickWorlds::Core::BLOCK_DIRT:
        r = 0.55f; g = 0.35f; b = 0.17f; break;
    case BrickWorlds::Core::BLOCK_STONE:
        r = 0.5f; g = 0.5f; b = 0.5f; break;
    case BrickWorlds::Core::BLOCK_WATER:
        r = 0.2f; g = 0.4f; b = 0.8f; break;
    case BrickWorlds::Core::BLOCK_WOOD:
        r = 0.4f; g = 0.25f; b = 0.1f; break;
    default:
        r = 1.0f; g = 0.0f; b = 1.0f; // Magenta for unknown
    }
}

void ChunkMesh::addCube(float x, float y, float z, float r, float g, float b, std::vector<Vertex>& vertices) {
    // Simple cube (6 faces, 2 triangles per face = 36 vertices)
    // Front face
    vertices.push_back({ {x, y, z + 1}, {r, g, b} });
    vertices.push_back({ {x + 1, y, z + 1}, {r, g, b} });
    vertices.push_back({ {x + 1, y + 1, z + 1}, {r, g, b} });
    vertices.push_back({ {x, y, z + 1}, {r, g, b} });
    vertices.push_back({ {x + 1, y + 1, z + 1}, {r, g, b} });
    vertices.push_back({ {x, y + 1, z + 1}, {r, g, b} });

    // Back face
    vertices.push_back({ {x + 1, y, z}, {r * 0.7f, g * 0.7f, b * 0.7f} });
    vertices.push_back({ {x, y, z}, {r * 0.7f, g * 0.7f, b * 0.7f} });
    vertices.push_back({ {x, y + 1, z}, {r * 0.7f, g * 0.7f, b * 0.7f} });
    vertices.push_back({ {x + 1, y, z}, {r * 0.7f, g * 0.7f, b * 0.7f} });
    vertices.push_back({ {x, y + 1, z}, {r * 0.7f, g * 0.7f, b * 0.7f} });
    vertices.push_back({ {x + 1, y + 1, z}, {r * 0.7f, g * 0.7f, b * 0.7f} });

    // Top face
    vertices.push_back({ {x, y + 1, z + 1}, {r * 0.9f, g * 0.9f, b * 0.9f} });
    vertices.push_back({ {x + 1, y + 1, z + 1}, {r * 0.9f, g * 0.9f, b * 0.9f} });
    vertices.push_back({ {x + 1, y + 1, z}, {r * 0.9f, g * 0.9f, b * 0.9f} });
    vertices.push_back({ {x, y + 1, z + 1}, {r * 0.9f, g * 0.9f, b * 0.9f} });
    vertices.push_back({ {x + 1, y + 1, z}, {r * 0.9f, g * 0.9f, b * 0.9f} });
    vertices.push_back({ {x, y + 1, z}, {r * 0.9f, g * 0.9f, b * 0.9f} });

    // Bottom face
    vertices.push_back({ {x, y, z}, {r * 0.5f, g * 0.5f, b * 0.5f} });
    vertices.push_back({ {x + 1, y, z}, {r * 0.5f, g * 0.5f, b * 0.5f} });
    vertices.push_back({ {x + 1, y, z + 1}, {r * 0.5f, g * 0.5f, b * 0.5f} });
    vertices.push_back({ {x, y, z}, {r * 0.5f, g * 0.5f, b * 0.5f} });
    vertices.push_back({ {x + 1, y, z + 1}, {r * 0.5f, g * 0.5f, b * 0.5f} });
    vertices.push_back({ {x, y, z + 1}, {r * 0.5f, g * 0.5f, b * 0.5f} });

    // Right face
    vertices.push_back({ {x + 1, y, z + 1}, {r * 0.8f, g * 0.8f, b * 0.8f} });
    vertices.push_back({ {x + 1, y, z}, {r * 0.8f, g * 0.8f, b * 0.8f} });
    vertices.push_back({ {x + 1, y + 1, z}, {r * 0.8f, g * 0.8f, b * 0.8f} });
    vertices.push_back({ {x + 1, y, z + 1}, {r * 0.8f, g * 0.8f, b * 0.8f} });
    vertices.push_back({ {x + 1, y + 1, z}, {r * 0.8f, g * 0.8f, b * 0.8f} });
    vertices.push_back({ {x + 1, y + 1, z + 1}, {r * 0.8f, g * 0.8f, b * 0.8f} });

    // Left face
    vertices.push_back({ {x, y, z}, {r * 0.6f, g * 0.6f, b * 0.6f} });
    vertices.push_back({ {x, y, z + 1}, {r * 0.6f, g * 0.6f, b * 0.6f} });
    vertices.push_back({ {x, y + 1, z + 1}, {r * 0.6f, g * 0.6f, b * 0.6f} });
    vertices.push_back({ {x, y, z}, {r * 0.6f, g * 0.6f, b * 0.6f} });
    vertices.push_back({ {x, y + 1, z + 1}, {r * 0.6f, g * 0.6f, b * 0.6f} });
    vertices.push_back({ {x, y + 1, z}, {r * 0.6f, g * 0.6f, b * 0.6f} });
}

void ChunkMesh::generate(const BrickWorlds::Core::Chunk& chunk) {
    std::vector<Vertex> vertices;

    using namespace BrickWorlds::Core;

    // Simple approach: render all non-air blocks (no culling for now)
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_HEIGHT; z++) {
                BlockID id = chunk.getBlock(x, y, z);
                if (id == BLOCK_AIR) continue;

                float r, g, b;
                getBlockColor(id, r, g, b);

                // Add cube at world position
                Vector3i chunkPos = chunk.getPosition();
                float worldX = chunkPos.x * CHUNK_SIZE + x;
                float worldY = chunkPos.y * CHUNK_SIZE + y;
                float worldZ = chunkPos.z * CHUNK_HEIGHT + z;

                addCube(worldX, worldY, worldZ, r, g, b, vertices);
            }
        }
    }

    m_vertexCount = vertices.size();

    if (m_vertexCount == 0) return;

    // Upload to GPU
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)0);

    // Color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);
}

void ChunkMesh::render() const {
    if (m_vertexCount == 0) return;

    glBindVertexArray(m_vao);
    glDrawArrays(0x0004, 0, m_vertexCount); // GL_TRIANGLES = 0x0004
    glBindVertexArray(0);
}

