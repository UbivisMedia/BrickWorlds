#pragma once

#include <cstdint>
#include <cmath>

namespace BrickWorlds {
namespace Core {

// Block ID type - 16 bits allows 65536 different block types
using BlockID = uint16_t;

// Special block IDs
const BlockID BLOCK_AIR = 0;
const BlockID BLOCK_DIRT = 1;
const BlockID BLOCK_STONE = 2;
const BlockID BLOCK_WATER = 3;
const BlockID BLOCK_WOOD = 4;

// Chunk dimensions (16x16x16 blocks per chunk)
const int CHUNK_SIZE = 16;
const int CHUNK_HEIGHT = 16;
const int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT;

// 3D Integer vector for block/chunk positions
struct Vector3i {
    int x, y, z;
    
    Vector3i() : x(0), y(0), z(0) {}
    Vector3i(int x, int y, int z) : x(x), y(y), z(z) {}
    
    bool operator==(const Vector3i& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    
    bool operator!=(const Vector3i& other) const {
        return !(*this == other);
    }
    
    Vector3i operator+(const Vector3i& other) const {
        return Vector3i(x + other.x, y + other.y, z + other.z);
    }
    
    Vector3i operator-(const Vector3i& other) const {
        return Vector3i(x - other.x, y - other.y, z - other.z);
    }
};

// 3D Float vector for positions, directions
struct Vector3f {
    float x, y, z;
    
    Vector3f() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}
    
    Vector3f operator+(const Vector3f& other) const {
        return Vector3f(x + other.x, y + other.y, z + other.z);
    }
    
    Vector3f operator-(const Vector3f& other) const {
        return Vector3f(x - other.x, y - other.y, z - other.z);
    }
    
    Vector3f operator*(float scalar) const {
        return Vector3f(x * scalar, y * scalar, z * scalar);
    }
    
    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    
    Vector3f normalized() const {
        float len = length();
        if (len > 0.0f) {
            return *this * (1.0f / len);
        }
        return *this;
    }
};

// Hash function for Vector3i (for use in unordered_map)
struct Vector3iHash {
    std::size_t operator()(const Vector3i& v) const {
        // Simple hash combination
        return std::hash<int>()(v.x) ^ 
               (std::hash<int>()(v.y) << 1) ^ 
               (std::hash<int>()(v.z) << 2);
    }
};

} // namespace Core
} // namespace BrickWorlds
