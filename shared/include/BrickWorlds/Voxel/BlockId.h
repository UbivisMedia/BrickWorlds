#pragma once
#include <cstdint>

namespace BrickWorlds::Voxel {

    using BlockId = std::uint16_t;

    enum : BlockId {
        Air = 0,
        Dirt = 1,
        Rock = 2,
        Water = 3,
    };

    // Startwerte (später konfigurierbar / serverseitig erzwungen)
    inline constexpr int ChunkX = 16;
    inline constexpr int ChunkY = 256;
    inline constexpr int ChunkZ = 16;

    inline constexpr int ChunkVolume = ChunkX * ChunkY * ChunkZ;

    inline constexpr int Index(int lx, int ly, int lz) {
        // (ly * Z + lz) * X + lx
        return (ly * ChunkZ + lz) * ChunkX + lx;
    }

} // namespace BrickWorlds::Voxel
