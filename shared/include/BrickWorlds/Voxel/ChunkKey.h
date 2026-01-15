#pragma once
#include <cstdint>
#include <functional>

namespace BrickWorlds::Voxel {

    struct ChunkKey {
        std::int32_t cx = 0;
        std::int32_t cz = 0;

        friend bool operator==(const ChunkKey& a, const ChunkKey& b) {
            return a.cx == b.cx && a.cz == b.cz;
        }
    };

    struct ChunkKeyHash {
        std::size_t operator()(const ChunkKey& k) const noexcept {
            std::uint64_t x = static_cast<std::uint32_t>(k.cx);
            std::uint64_t z = static_cast<std::uint32_t>(k.cz);
            std::uint64_t h = (x << 32) ^ z;

            // splitmix64
            h += 0x9e3779b97f4a7c15ull;
            h = (h ^ (h >> 30)) * 0xbf58476d1ce4e5b9ull;
            h = (h ^ (h >> 27)) * 0x94d049bb133111ebull;
            h = (h ^ (h >> 31));
            return static_cast<std::size_t>(h);
        }
    };

} // namespace BrickWorlds::Voxel
