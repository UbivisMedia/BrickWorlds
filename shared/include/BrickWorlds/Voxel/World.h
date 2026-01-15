#pragma once
#include <cstdint>
#include <functional>
#include <memory>

#include "ChunkManager.h"
#include "Jobs.h"

namespace BrickWorlds::Voxel {

    struct IChunkGenerator {
        virtual ~IChunkGenerator() = default;
        virtual void Generate(Chunk& chunk) = 0;
    };

    class World {
    public:
        explicit World(IChunkGenerator* generator);

        // Streaming: Worker-Queues
        void StartStreaming(std::size_t genThreads, std::size_t meshThreads);
        void StopStreaming();

        // Chunk Streaming: lädt/generiert Chunks im Radius um Player-Position (Blocks)
        void UpdateStreaming(int playerWx, int playerWz, int viewDistanceChunks);

        // Block API
        BlockId GetBlock(int wx, int wy, int wz) const;
        void SetBlock(int wx, int wy, int wz, BlockId id);

        ChunkManager& Chunks() { return chunks_; }
        const ChunkManager& Chunks() const { return chunks_; }

        static ChunkKey WorldToChunk(int wx, int wz);
        static void WorldToLocal(int wx, int wy, int wz, int& lx, int& ly, int& lz);

    private:
        void EnqueueGenerate(const std::shared_ptr<Chunk>& ch);
        void EnqueueMesh(const std::shared_ptr<Chunk>& ch);

        void MarkNeighborsDirtyIfEdge(const ChunkKey& ck, int lx, int lz);

        ChunkManager chunks_;
        IChunkGenerator* generator_ = nullptr;

        JobQueue genQ_;
        JobQueue meshQ_;
    };

} // namespace BrickWorlds::Voxel
