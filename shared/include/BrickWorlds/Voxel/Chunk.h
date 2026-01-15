#pragma once
#include <atomic>
#include <cstdint>
#include <mutex>
#include <vector>

#include "BlockId.h"
#include "ChunkKey.h"

namespace BrickWorlds::Voxel {

    enum class ChunkState : std::uint8_t {
        Empty = 0,
        Generating,
        ReadyData,
        Meshing,
        ReadyMesh,
        Unloading
    };

    struct ChunkMeshData {
        // Placeholder: später ersetzen durch echte Vertex-Structs / GPU-Handles
        std::vector<float> vertices;
        std::vector<std::uint32_t> indices;

        void Clear() { vertices.clear(); indices.clear(); }
    };

    class Chunk {
    public:
        explicit Chunk(ChunkKey key);

        const ChunkKey& Key() const { return key_; }

        ChunkState State() const { return state_.load(std::memory_order_relaxed); }
        void SetState(ChunkState s) { state_.store(s, std::memory_order_relaxed); }

        // Simple thread-safe access (später optimierbar)
        BlockId Get(int lx, int ly, int lz) const;
        void Set(int lx, int ly, int lz, BlockId id);

        // Für Generator/Mesher: extern synchronisieren über Mutex()
        std::vector<BlockId>& BlocksUnsafe() { return blocks_; }
        const std::vector<BlockId>& BlocksUnsafe() const { return blocks_; }

        bool ConsumeDirtyBlocks() { return dirtyBlocks_.exchange(false, std::memory_order_relaxed); }
        void MarkDirtyMesh() { dirtyMesh_.store(true, std::memory_order_relaxed); }
        bool ConsumeDirtyMesh() { return dirtyMesh_.exchange(false, std::memory_order_relaxed); }

        ChunkMeshData& Mesh() { return mesh_; }
        const ChunkMeshData& Mesh() const { return mesh_; }

        std::mutex& Mutex() { return mtx_; }

    private:
        ChunkKey key_;
        mutable std::mutex mtx_;
        std::vector<BlockId> blocks_;
        ChunkMeshData mesh_;

        std::atomic<ChunkState> state_{ ChunkState::Empty };
        std::atomic<bool> dirtyBlocks_{ true }; // initial: needs mesh after generate
        std::atomic<bool> dirtyMesh_{ true };
    };

} // namespace BrickWorlds::Voxel
