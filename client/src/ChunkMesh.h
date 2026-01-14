#pragma once

#include <BrickWorlds/Core/Chunk.h>
#include <vector>

struct Vertex {
    float position[3];
    float color[3];
};

class ChunkMesh {
public:
    ChunkMesh();
    ~ChunkMesh();

    void generate(const BrickWorlds::Core::Chunk& chunk);
    void render() const;

    bool isEmpty() const { return m_vertexCount == 0; }

private:
    unsigned int m_vao;
    unsigned int m_vbo;
    int m_vertexCount;

    void addCube(float x, float y, float z, float r, float g, float b, std::vector<Vertex>& vertices);
    void getBlockColor(BrickWorlds::Core::BlockID id, float& r, float& g, float& b);
};
