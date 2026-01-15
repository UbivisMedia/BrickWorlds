#pragma once

#include "Shader.h"
#include "Camera.h"
#include "ChunkMesh.h"
#include <BrickWorlds/Voxel/World.h>
#include <BrickWorlds/Voxel/ChunkKey.h>
#include <unordered_map>

struct ChunkKeyHasher {
    std::size_t operator()(const BrickWorlds::Voxel::ChunkKey& k) const noexcept {
        return BrickWorlds::Voxel::ChunkKeyHash{}(k);
    }
};

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool initialize();
    void render(const BrickWorlds::Voxel::World& world, const Camera& camera);

private:
    Shader m_shader;
    std::unordered_map<BrickWorlds::Voxel::ChunkKey, ChunkMesh*, ChunkKeyHasher> m_chunkMeshes;

    void updateChunkMeshes(const BrickWorlds::Voxel::World& world, const Camera& camera);
};
