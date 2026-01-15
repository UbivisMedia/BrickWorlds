#include "BrickWorlds/Voxel/ChunkManager.h"

namespace BrickWorlds::Voxel {

    std::shared_ptr<Chunk> ChunkManager::GetChunk(const ChunkKey& key) const {
        std::shared_lock lk(mtx_);
        auto it = chunks_.find(key);
        return (it != chunks_.end()) ? it->second : nullptr;
    }

    std::shared_ptr<Chunk> ChunkManager::GetOrCreate(const ChunkKey& key) {
        {
            std::shared_lock lk(mtx_);
            auto it = chunks_.find(key);
            if (it != chunks_.end()) return it->second;
        }
        std::unique_lock lk(mtx_);
        auto& ref = chunks_[key];
        if (!ref) ref = std::make_shared<Chunk>(key);
        return ref;
    }

    void ChunkManager::Remove(const ChunkKey& key) {
        std::unique_lock lk(mtx_);
        chunks_.erase(key);
    }

    std::vector<std::shared_ptr<Chunk>> ChunkManager::SnapshotAll() const {
        std::shared_lock lk(mtx_);
        std::vector<std::shared_ptr<Chunk>> out;
        out.reserve(chunks_.size());
        for (auto& kv : chunks_) out.push_back(kv.second);
        return out;
    }

} // namespace BrickWorlds::Voxel
