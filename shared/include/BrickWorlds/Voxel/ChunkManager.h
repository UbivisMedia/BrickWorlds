#pragma once
#include <memory>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

#include "Chunk.h"

namespace BrickWorlds::Voxel {

    class ChunkManager {
    public:
        std::shared_ptr<Chunk> GetChunk(const ChunkKey& key) const;
        std::shared_ptr<Chunk> GetOrCreate(const ChunkKey& key);

        void Remove(const ChunkKey& key);
        std::vector<std::shared_ptr<Chunk>> SnapshotAll() const;

    private:
        mutable std::shared_mutex mtx_;
        std::unordered_map<ChunkKey, std::shared_ptr<Chunk>, ChunkKeyHash> chunks_;
    };

} // namespace BrickWorlds::Voxel
