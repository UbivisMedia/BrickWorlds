#pragma once

#include <BrickWorlds/Voxel/Chunk.h>
#include <BrickWorlds/Voxel/BlockId.h>
#include <vector>
#include <cstdint>

namespace BrickWorlds::Voxel {
    class World; // <- Forward Declaration (WICHTIG)
}

struct Vertex {
    float position[3];
    float color[3];
};

class ChunkMesh {
public:
    ChunkMesh();
    ~ChunkMesh();

    void generate(const BrickWorlds::Voxel::World& world,
        const BrickWorlds::Voxel::Chunk& chunk);

    void render() const;

    bool isEmpty() const { return m_vertexCount == 0; }

private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    int m_vertexCount = 0;

    void addFace(int face, float x, float y, float z, float r, float g, float b, std::vector<Vertex>& vertices);
    void getBlockColor(BrickWorlds::Voxel::BlockId id, float& r, float& g, float& b);
};
