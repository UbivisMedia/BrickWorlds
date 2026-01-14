#pragma once

#include "Chunk.h"
#include "Types.h"
#include <unordered_map>
#include <memory>

namespace BrickWorlds {
namespace Core {

// World class - manages all chunks and provides world-level operations
class World {
public:
    World();
    ~World() = default;

    // Get or create a chunk at the given chunk coordinates
    Chunk* getChunk(const Vector3i& chunkPos);
    
    // Get block in world coordinates
    BlockID getBlock(int x, int y, int z);
    
    // Set block in world coordinates
    void setBlock(int x, int y, int z, BlockID blockId);
    
    // Generate a simple flat world for testing
    void generateFlatWorld(int radius = 8, int height = 4);
    
    // Get all loaded chunks
    const std::unordered_map<Vector3i, std::unique_ptr<Chunk>, Vector3iHash>& getChunks() const {
        return m_chunks;
    }
    
private:
    std::unordered_map<Vector3i, std::unique_ptr<Chunk>, Vector3iHash> m_chunks;
    
    // Convert world coordinates to chunk coordinates
    Vector3i worldToChunk(int x, int y, int z) const;
    
    // Convert world coordinates to local chunk coordinates
    Vector3i worldToLocal(int x, int y, int z) const;
};

// World implementation
inline World::World() {
}

inline Vector3i World::worldToChunk(int x, int y, int z) const {
    // Integer division rounds toward negative infinity
    int cx = (x < 0) ? ((x + 1) / CHUNK_SIZE - 1) : (x / CHUNK_SIZE);
    int cy = (y < 0) ? ((y + 1) / CHUNK_SIZE - 1) : (y / CHUNK_SIZE);
    int cz = (z < 0) ? ((z + 1) / CHUNK_HEIGHT - 1) : (z / CHUNK_HEIGHT);
    return Vector3i(cx, cy, cz);
}

inline Vector3i World::worldToLocal(int x, int y, int z) const {
    int lx = x & (CHUNK_SIZE - 1);  // x % CHUNK_SIZE
    int ly = y & (CHUNK_SIZE - 1);
    int lz = z & (CHUNK_HEIGHT - 1);
    if (x < 0 && lx != 0) lx = CHUNK_SIZE + lx;
    if (y < 0 && ly != 0) ly = CHUNK_SIZE + ly;
    if (z < 0 && lz != 0) lz = CHUNK_HEIGHT + lz;
    return Vector3i(lx, ly, lz);
}

inline Chunk* World::getChunk(const Vector3i& chunkPos) {
    auto it = m_chunks.find(chunkPos);
    if (it != m_chunks.end()) {
        return it->second.get();
    }
    
    // Create new chunk
    auto chunk = std::make_unique<Chunk>(chunkPos);
    Chunk* ptr = chunk.get();
    m_chunks[chunkPos] = std::move(chunk);
    return ptr;
}

inline BlockID World::getBlock(int x, int y, int z) {
    Vector3i chunkPos = worldToChunk(x, y, z);
    Chunk* chunk = getChunk(chunkPos);
    if (!chunk) return BLOCK_AIR;
    
    Vector3i local = worldToLocal(x, y, z);
    return chunk->getBlock(local.x, local.y, local.z);
}

inline void World::setBlock(int x, int y, int z, BlockID blockId) {
    Vector3i chunkPos = worldToChunk(x, y, z);
    Chunk* chunk = getChunk(chunkPos);
    if (!chunk) return;
    
    Vector3i local = worldToLocal(x, y, z);
    chunk->setBlock(local.x, local.y, local.z, blockId);
}

inline void World::generateFlatWorld(int radius, int height) {
    // Generate a simple flat world for testing
    for (int cx = -radius; cx < radius; cx++) {
        for (int cz = -radius; cz < radius; cz++) {
            Chunk* chunk = getChunk(Vector3i(cx, 0, cz));
            
            // Fill bottom layers with stone, top layer with dirt
            for (int x = 0; x < CHUNK_SIZE; x++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    for (int y = 0; y < height && y < CHUNK_SIZE; y++) {
                        if (y < height - 1) {
                            chunk->setBlock(x, y, z, BLOCK_STONE);
                        } else {
                            chunk->setBlock(x, y, z, BLOCK_DIRT);
                        }
                    }
                }
            }
        }
    }
}

} // namespace Core
} // namespace BrickWorlds
