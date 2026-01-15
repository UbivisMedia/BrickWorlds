#pragma once
#include "World.h"
#include "BlockId.h"

namespace BrickWorlds::Voxel {

    class FlatGenerator final : public IChunkGenerator {
    public:
        void Generate(Chunk& chunk) override {
            // Simple: y < 60 Dirt, y==60 Grass (später), darüber Air
            auto& b = chunk.BlocksUnsafe();
            for (int y = 0; y < ChunkY; ++y) {
                for (int z = 0; z < ChunkZ; ++z) {
                    for (int x = 0; x < ChunkX; ++x) {
                        BlockId id = Air;
                        if (y < 58) id = Rock;
                        else if (y < 60) id = Dirt;
                        b[Index(x, y, z)] = id;
                    }
                }
            }
        }
    };

} // namespace BrickWorlds::Voxel
