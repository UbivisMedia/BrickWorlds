#include "ChunkMesh.h"
#include <BrickWorlds/Voxel/World.h>
#include <GL/glew.h>

using namespace BrickWorlds::Voxel;

ChunkMesh::ChunkMesh() {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
}

ChunkMesh::~ChunkMesh() {
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

void ChunkMesh::getBlockColor(BlockId id, float& r, float& g, float& b) {
    switch (id) {
        case Dirt:  r = 0.55f; g = 0.35f; b = 0.17f; break;
        case Rock:  r = 0.50f; g = 0.50f; b = 0.50f; break;
        case Water: r = 0.20f; g = 0.40f; b = 0.80f; break;
        default:    r = 1.00f; g = 0.00f; b = 1.00f; break;
    }
}

void ChunkMesh::addFace(int face, float x, float y, float z, float r, float g, float b, std::vector<Vertex>& v) {
    auto push = [&](float px, float py, float pz, float cr, float cg, float cb) {
        v.push_back({ {px, py, pz}, {cr, cg, cb} });
    };

    switch (face) {
        case 0: // +Z
            push(x, y, z + 1, r, g, b);
            push(x + 1, y, z + 1, r, g, b);
            push(x + 1, y + 1, z + 1, r, g, b);
            push(x, y, z + 1, r, g, b);
            push(x + 1, y + 1, z + 1, r, g, b);
            push(x, y + 1, z + 1, r, g, b);
            break;
        case 1: // -Z (dunkler)
            push(x + 1, y, z, r * 0.7f, g * 0.7f, b * 0.7f);
            push(x, y, z, r * 0.7f, g * 0.7f, b * 0.7f);
            push(x, y + 1, z, r * 0.7f, g * 0.7f, b * 0.7f);
            push(x + 1, y, z, r * 0.7f, g * 0.7f, b * 0.7f);
            push(x, y + 1, z, r * 0.7f, g * 0.7f, b * 0.7f);
            push(x + 1, y + 1, z, r * 0.7f, g * 0.7f, b * 0.7f);
            break;
        case 2: // +Y (leicht heller)
            push(x, y + 1, z + 1, r * 0.9f, g * 0.9f, b * 0.9f);
            push(x + 1, y + 1, z + 1, r * 0.9f, g * 0.9f, b * 0.9f);
            push(x + 1, y + 1, z, r * 0.9f, g * 0.9f, b * 0.9f);
            push(x, y + 1, z + 1, r * 0.9f, g * 0.9f, b * 0.9f);
            push(x + 1, y + 1, z, r * 0.9f, g * 0.9f, b * 0.9f);
            push(x, y + 1, z, r * 0.9f, g * 0.9f, b * 0.9f);
            break;
        case 3: // -Y (dunkler)
            push(x, y, z, r * 0.5f, g * 0.5f, b * 0.5f);
            push(x + 1, y, z, r * 0.5f, g * 0.5f, b * 0.5f);
            push(x + 1, y, z + 1, r * 0.5f, g * 0.5f, b * 0.5f);
            push(x, y, z, r * 0.5f, g * 0.5f, b * 0.5f);
            push(x + 1, y, z + 1, r * 0.5f, g * 0.5f, b * 0.5f);
            push(x, y, z + 1, r * 0.5f, g * 0.5f, b * 0.5f);
            break;
        case 4: // +X
            push(x + 1, y, z + 1, r * 0.8f, g * 0.8f, b * 0.8f);
            push(x + 1, y, z, r * 0.8f, g * 0.8f, b * 0.8f);
            push(x + 1, y + 1, z, r * 0.8f, g * 0.8f, b * 0.8f);
            push(x + 1, y, z + 1, r * 0.8f, g * 0.8f, b * 0.8f);
            push(x + 1, y + 1, z, r * 0.8f, g * 0.8f, b * 0.8f);
            push(x + 1, y + 1, z + 1, r * 0.8f, g * 0.8f, b * 0.8f);
            break;
        case 5: // -X
            push(x, y, z, r * 0.6f, g * 0.6f, b * 0.6f);
            push(x, y, z + 1, r * 0.6f, g * 0.6f, b * 0.6f);
            push(x, y + 1, z + 1, r * 0.6f, g * 0.6f, b * 0.6f);
            push(x, y, z, r * 0.6f, g * 0.6f, b * 0.6f);
            push(x, y + 1, z + 1, r * 0.6f, g * 0.6f, b * 0.6f);
            push(x, y + 1, z, r * 0.6f, g * 0.6f, b * 0.6f);
            break;
        default:
            break;
    }
}

void ChunkMesh::generate(const BrickWorlds::Voxel::World& world,
                         const BrickWorlds::Voxel::Chunk& chunk)
{
    using namespace BrickWorlds::Voxel;
    std::vector<Vertex> vertices;
    vertices.reserve(20000);

    const auto& key = chunk.Key();
    const int baseX = key.cx * ChunkX;
    const int baseZ = key.cz * ChunkZ;

    // *** PERFORMANCE: Cache neighbor chunks to avoid repeated HashMap lookups ***
    auto chunkPlusX  = world.Chunks().GetChunk({key.cx + 1, key.cz});
    auto chunkMinusX = world.Chunks().GetChunk({key.cx - 1, key.cz});
    auto chunkPlusZ  = world.Chunks().GetChunk({key.cx, key.cz + 1});
    auto chunkMinusZ = world.Chunks().GetChunk({key.cx, key.cz - 1});

    // Lambda for fast neighbor checking without World::GetBlock()
    auto isAir = [&](int lx, int ly, int lz) -> bool {
        // Check Y bounds
        if (ly < 0) return true;  // Below world = air
        if (ly >= ChunkY) return true;  // Above world = air

        // Inside current chunk?
        if (lx >= 0 && lx < ChunkX && lz >= 0 && lz < ChunkZ) {
            return chunk.Get(lx, ly, lz) == Air;
        }

        // Check neighbor chunks directly (no HashMap lookup!)
        // Check neighbor chunks directly (no HashMap lookup!)
        // Only check ONE axis at a time - others must be in valid range!
        if (lx < 0 && lz >= 0 && lz < ChunkZ && chunkMinusX) {
            return chunkMinusX->Get(ChunkX - 1, ly, lz) == Air;
        }
        if (lx >= ChunkX && lz >= 0 && lz < ChunkZ && chunkPlusX) {
            return chunkPlusX->Get(0, ly, lz) == Air;
        }
        if (lz < 0 && lx >= 0 && lx < ChunkX && chunkMinusZ) {
            return chunkMinusZ->Get(lx, ly, ChunkZ - 1) == Air;
        }
        if (lz >= ChunkZ && lx >= 0 && lx < ChunkX && chunkPlusZ) {
            return chunkPlusZ->Get(lx, ly, 0) == Air;
        }
        // Unloaded chunk = treat as solid (render face)      
        return false;
    for (int lx = 0; lx < ChunkX; ++lx) {
        for (int lz = 0; lz < ChunkZ; ++lz) {
            for (int y = 0; y < ChunkY; ++y) {
                const int wx = baseX + lx;
                const int wz = baseZ + lz;

                // Read block directly from chunk (no World lookup)
                BlockId id = chunk.Get(lx, y, lz);
                if (id == Air) continue;

                float r, g, b;
                getBlockColor(id, r, g, b);

                // Check all 6 faces using fast isAir lambda
                if (isAir(lx, y, lz + 1)) addFace(0, (float)wx, (float)y, (float)wz, r, g, b, vertices);
                if (isAir(lx, y, lz - 1)) addFace(1, (float)wx, (float)y, (float)wz, r, g, b, vertices);
                if (isAir(lx, y + 1, lz)) addFace(2, (float)wx, (float)y, (float)wz, r, g, b, vertices);
                if (isAir(lx, y - 1, lz)) addFace(3, (float)wx, (float)y, (float)wz, r, g, b, vertices);
                if (isAir(lx + 1, y, lz)) addFace(4, (float)wx, (float)y, (float)wz, r, g, b, vertices);
                if (isAir(lx - 1, y, lz)) addFace(5, (float)wx, (float)y, (float)wz, r, g, b, vertices);
            }
        }
    }

    m_vertexCount = (int)vertices.size();
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
