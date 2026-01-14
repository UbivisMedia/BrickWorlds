#include "Renderer.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#elif defined(__linux__)
#include <GL/gl.h>
#endif

// Embedded shaders
const char* vertexShaderSource = R"(
#version 120

attribute vec3 aPos;
attribute vec3 aColor;

varying vec3 fragColor;

uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    gl_Position = uProjection * uView * vec4(aPos, 1.0);
    fragColor = aColor;
}
)";

const char* fragmentShaderSource = R"(
#version 120

varying vec3 fragColor;

void main() {
    gl_FragColor = vec4(fragColor, 1.0);
}
)";

Renderer::Renderer() {
}

Renderer::~Renderer() {
    for (auto& pair : m_chunkMeshes) {
        delete pair.second;
    }
}

bool Renderer::initialize() {
    // Load shader
    if (!m_shader.loadFromStrings(vertexShaderSource, fragmentShaderSource)) {
        std::cerr << "Failed to load shaders!" << std::endl;
        return false;
    }

    // Enable depth testing
    glEnable(0x0B71); // GL_DEPTH_TEST

    std::cout << "Renderer initialized successfully" << std::endl;
    return true;
}

void Renderer::updateChunkMeshes(const BrickWorlds::Core::World& world) {
    // Generate meshes for dirty chunks
    for (const auto& pair : world.getChunks()) {
        const BrickWorlds::Core::Vector3i& pos = pair.first;
        const BrickWorlds::Core::Chunk* chunk = pair.second.get();

        if (chunk->isDirty()) {
            // Create or update mesh
            ChunkMesh* mesh = nullptr;
            auto it = m_chunkMeshes.find(pos);
            if (it != m_chunkMeshes.end()) {
                mesh = it->second;
            }
            else {
                mesh = new ChunkMesh();
                m_chunkMeshes[pos] = mesh;
            }

            mesh->generate(*chunk);
            const_cast<BrickWorlds::Core::Chunk*>(chunk)->setDirty(false);
        }
    }
}

void Renderer::render(const BrickWorlds::Core::World& world, const Camera& camera) {
    // Clear screen
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Sky blue
    glClear(0x00004100); // GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT

    // Update meshes
    updateChunkMeshes(world);

    // Use shader
    m_shader.use();

    // Set camera matrices
    float viewMatrix[16];
    float projectionMatrix[16];
    camera.getViewMatrix(viewMatrix);
    camera.getProjectionMatrix(projectionMatrix);

    m_shader.setMat4("uView", viewMatrix);
    m_shader.setMat4("uProjection", projectionMatrix);

    // Render all chunk meshes
    for (const auto& pair : m_chunkMeshes) {
        pair.second->render();
    }
}
