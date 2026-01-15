#include "BrickWorlds/Voxel/Chunk.h"

namespace BrickWorlds::Voxel {

    Chunk::Chunk(ChunkKey key)
        : key_(key),
        blocks_(ChunkVolume, BlockId{ Air }) {
    }

    BlockId Chunk::Get(int lx, int ly, int lz) const {
        std::scoped_lock lk(mtx_);
        return blocks_[Index(lx, ly, lz)];
    }

    void Chunk::Set(int lx, int ly, int lz, BlockId id) {
        std::scoped_lock lk(mtx_);
        blocks_[Index(lx, ly, lz)] = id;
        dirtyBlocks_.store(true, std::memory_order_relaxed);
        dirtyMesh_.store(true, std::memory_order_relaxed);
    }

} // namespace BrickWorlds::Voxel
