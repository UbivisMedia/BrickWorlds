#pragma once

#include "Shader.h"
#include "Camera.h"
#include "ChunkMesh.h"
#include <BrickWorlds/Core/World.h>
#include <unordered_map>

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool initialize();
    void render(const BrickWorlds::Core::World& world, const Camera& camera);

private:
    Shader m_shader;
    std::unordered_map<BrickWorlds::Core::Vector3i, ChunkMesh*, BrickWorlds::Core::Vector3iHash> m_chunkMeshes;

    void updateChunkMeshes(const BrickWorlds::Core::World& world);
};
