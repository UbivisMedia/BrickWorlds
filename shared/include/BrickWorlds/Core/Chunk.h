#pragma once

#include "Types.h"
#include <array>
#include <memory>

namespace BrickWorlds {
namespace Core {

// Chunk class - holds a 16x16x16 block of voxels
class Chunk {
public:
    Chunk(const Vector3i& position);
    ~Chunk() = default;

    // Get/Set block at local coordinates (0-15)
    BlockID getBlock(int x, int y, int z) const;
    void setBlock(int x, int y, int z, BlockID blockId);
    
    // Get chunk world position
    const Vector3i& getPosition() const { return m_position; }
    
    // Check if chunk needs mesh rebuild
    bool isDirty() const { return m_dirty; }
    void setDirty(bool dirty) { m_dirty = dirty; }
    
    // Direct access to block data (for meshing)
    const BlockID* getData() const { return m_blocks.data(); }
    
private:
    Vector3i m_position;  // Chunk position in world space
    std::array<BlockID, CHUNK_VOLUME> m_blocks;  // 16x16x16 blocks
    bool m_dirty;  // Needs mesh rebuild
    
    // Convert 3D coordinates to 1D array index
    inline int getIndex(int x, int y, int z) const {
        return x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE;
    }
};

// Chunk implementation
inline Chunk::Chunk(const Vector3i& position) 
    : m_position(position), m_dirty(true) {
    // Initialize all blocks to air
    m_blocks.fill(BLOCK_AIR);
}

inline BlockID Chunk::getBlock(int x, int y, int z) const {
    if (x < 0 || x >= CHUNK_SIZE ||
        y < 0 || y >= CHUNK_SIZE ||
        z < 0 || z >= CHUNK_HEIGHT) {
        return BLOCK_AIR;
    }
    return m_blocks[getIndex(x, y, z)];
}

inline void Chunk::setBlock(int x, int y, int z, BlockID blockId) {
    if (x < 0 || x >= CHUNK_SIZE ||
        y < 0 || y >= CHUNK_SIZE ||
        z < 0 || z >= CHUNK_HEIGHT) {
        return;
    }
    m_blocks[getIndex(x, y, z)] = blockId;
    m_dirty = true;  // Mark for mesh rebuild
}

} // namespace Core
} // namespace BrickWorlds
