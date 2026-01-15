#include "Renderer.h"
#include <iostream>
#include <GL/glew.h>
#include <algorithm>>
#include <chrono>

static const char* kVertexShader = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform mat4 uView;
uniform mat4 uProjection;

out vec3 vColor;

void main() {
    vColor = aColor;
    gl_Position = uProjection * uView * vec4(aPos, 1.0);
}
)";

static const char* kFragmentShader = R"(
#version 330 core
in vec3 vColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(vColor, 1.0);
}
)";

Renderer::Renderer() {}

Renderer::~Renderer() {
    for (auto& kv : m_chunkMeshes) {
        delete kv.second;
    }
    m_chunkMeshes.clear();
}

bool Renderer::initialize() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    if (!m_shader.loadFromStrings(kVertexShader, kFragmentShader)) {
        std::cerr << "Failed to compile/link embedded shader." << std::endl;
        return false;
    }
    return true;
}

void Renderer::updateChunkMeshes(const BrickWorlds::Voxel::World& world, const Camera& camera) {
    auto chunks = world.Chunks().SnapshotAll();

    // Kamera-Position für Distanzsortierung
    auto camPos = camera.getPosition();
    const int camWx = static_cast<int>(camPos.x);
    const int camWz = static_cast<int>(camPos.z);

    std::sort(chunks.begin(), chunks.end(),
        [&](const std::shared_ptr<BrickWorlds::Voxel::Chunk>& a,
            const std::shared_ptr<BrickWorlds::Voxel::Chunk>& b)
        {
            if (!a) return false;
            if (!b) return true;

            const auto ka = a->Key();
            const auto kb = b->Key();

            const int ax = ka.cx * BrickWorlds::Voxel::ChunkX + BrickWorlds::Voxel::ChunkX / 2;
            const int az = ka.cz * BrickWorlds::Voxel::ChunkZ + BrickWorlds::Voxel::ChunkZ / 2;
            const int bx = kb.cx * BrickWorlds::Voxel::ChunkX + BrickWorlds::Voxel::ChunkX / 2;
            const int bz = kb.cz * BrickWorlds::Voxel::ChunkZ + BrickWorlds::Voxel::ChunkZ / 2;

            const int da = (ax - camWx) * (ax - camWx) + (az - camWz) * (az - camWz);
            const int db = (bx - camWx) * (bx - camWx) + (bz - camWz) * (bz - camWz);

            return da < db;
        });

    int builtThisFrame = 0;
    const int maxBuildPerFrame = 6; // <- start: 4..8 fühlt meist gut an

    for (auto& ch : chunks) {
        if (!ch) continue;

        const auto st = ch->State();
        if (st != BrickWorlds::Voxel::ChunkState::ReadyData &&
            st != BrickWorlds::Voxel::ChunkState::ReadyMesh)
            continue;

        if (!ch->ConsumeDirtyMesh())
            continue;

        if (builtThisFrame >= maxBuildPerFrame)
            break;

        const auto key = ch->Key();

        ChunkMesh* mesh = nullptr;
        auto it = m_chunkMeshes.find(key);
        if (it == m_chunkMeshes.end()) {
            mesh = new ChunkMesh();
            m_chunkMeshes.emplace(key, mesh);
        }
        else {
            mesh = it->second;
        }

        mesh->generate(world, *ch);
        ++builtThisFrame;
    }
}


void Renderer::render(const BrickWorlds::Voxel::World& world, const Camera& camera) {
    updateChunkMeshes(world, camera);

    glClearColor(0.52f, 0.75f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader.use();

    float view[16];
    float proj[16];
    camera.getViewMatrix(view);
    camera.getProjectionMatrix(proj);

    m_shader.setMat4("uView", view);
    m_shader.setMat4("uProjection", proj);

    // --- Distanz-Culling (sehr günstig, großer Effekt) ---
    auto cp = camera.getPosition();
    const float camX = cp.x;
    const float camZ = cp.z;

    // Radius in Chunks; sollte grob zu main.cpp UpdateStreaming(..., viewDistanceChunks) passen
    const int viewDistanceChunks = 6;

    const float radius = float(viewDistanceChunks * BrickWorlds::Voxel::ChunkX);
    const float radius2 = radius * radius;

    for (const auto& kv : m_chunkMeshes) {
        if (!kv.second) continue;

        const auto& key = kv.first;

        // Chunk-Mitte in World-Koordinaten
        const float chunkCenterX = float(key.cx * BrickWorlds::Voxel::ChunkX + BrickWorlds::Voxel::ChunkX * 0.5f);
        const float chunkCenterZ = float(key.cz * BrickWorlds::Voxel::ChunkZ + BrickWorlds::Voxel::ChunkZ * 0.5f);

        const float dx = chunkCenterX - camX;
        const float dz = chunkCenterZ - camZ;

        if (dx * dx + dz * dz > radius2)
            continue;

        kv.second->render();
    }
}

