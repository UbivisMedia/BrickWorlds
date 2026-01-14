#include "ChunkMesh.h"
#include <iostream>
#include <GL/glew.h>

ChunkMesh::ChunkMesh() : m_vao(0), m_vbo(0), m_vertexCount(0) {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
}

ChunkMesh::~ChunkMesh() {
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
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
        r = 1.0f; g = 0.0f; b = 1.0f;
    }
}

void ChunkMesh::addCube(float x, float y, float z, float r, float g, float b, std::vector<Vertex>& vertices) {
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

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_HEIGHT; z++) {
                BlockID id = chunk.getBlock(x, y, z);
                if (id == BLOCK_AIR) continue;

                float r, g, b;
                getBlockColor(id, r, g, b);

                Vector3i chunkPos = chunk.getPosition();
                float worldX = static_cast<float>(chunkPos.x * CHUNK_SIZE + x);
                float worldY = static_cast<float>(chunkPos.y * CHUNK_SIZE + y);
                float worldZ = static_cast<float>(chunkPos.z * CHUNK_HEIGHT + z);

                addCube(worldX, worldY, worldZ, r, g, b, vertices);
            }
        }
    }

    m_vertexCount = static_cast<int>(vertices.size());

    if (m_vertexCount == 0) return;

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);
}

void ChunkMesh::render() const {
    if (m_vertexCount == 0) return;

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    glBindVertexArray(0);
}
