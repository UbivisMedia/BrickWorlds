#include "BrickWorlds/Voxel/World.h"
#include "BrickWorlds/Voxel/BlockId.h"

#include <algorithm>
#include <unordered_set>

namespace BrickWorlds::Voxel {

    static int FloorDiv(int a, int b) {
        int q = a / b;
        int r = a % b;
        if (r != 0 && ((r > 0) != (b > 0))) --q;
        return q;
    }

    static int Mod(int a, int b) {
        int r = a % b;
        return (r < 0) ? (r + b) : r;
    }

    World::World(IChunkGenerator* generator)
        : generator_(generator) {
    }

    void World::StartStreaming(std::size_t genThreads, std::size_t meshThreads) {
        genQ_.Start(std::max<std::size_t>(1, genThreads));
        meshQ_.Start(std::max<std::size_t>(1, meshThreads));
    }

    void World::StopStreaming() {
        genQ_.Stop();
        meshQ_.Stop();
    }

    ChunkKey World::WorldToChunk(int wx, int wz) {
        return ChunkKey{ static_cast<std::int32_t>(FloorDiv(wx, ChunkX)),
                         static_cast<std::int32_t>(FloorDiv(wz, ChunkZ)) };
    }

    void World::WorldToLocal(int wx, int wy, int wz, int& lx, int& ly, int& lz) {
        lx = Mod(wx, ChunkX);
        ly = wy;
        lz = Mod(wz, ChunkZ);
    }

    BlockId World::GetBlock(int wx, int wy, int wz) const {
        if (wy < 0 || wy >= ChunkY) return Air;

        ChunkKey ck = WorldToChunk(wx, wz);
        int lx, ly, lz;
        WorldToLocal(wx, wy, wz, lx, ly, lz);

        auto ch = chunks_.GetChunk(ck);
        if (!ch) return Air;
        return ch->Get(lx, ly, lz);
    }

    void World::MarkNeighborsDirtyIfEdge(const ChunkKey& ck, int lx, int lz) {
        // Wenn an der Chunk-Kante: Nachbarn remesh anstoßen (wenn geladen)
        if (lx == 0) {
            auto nb = chunks_.GetChunk({ ck.cx - 1, ck.cz });
            if (nb) nb->MarkDirtyMesh();
        }
        else if (lx == ChunkX - 1) {
            auto nb = chunks_.GetChunk({ ck.cx + 1, ck.cz });
            if (nb) nb->MarkDirtyMesh();
        }

        if (lz == 0) {
            auto nb = chunks_.GetChunk({ ck.cx, ck.cz - 1 });
            if (nb) nb->MarkDirtyMesh();
        }
        else if (lz == ChunkZ - 1) {
            auto nb = chunks_.GetChunk({ ck.cx, ck.cz + 1 });
            if (nb) nb->MarkDirtyMesh();
        }
    }

    void World::SetBlock(int wx, int wy, int wz, BlockId id) {
        if (wy < 0 || wy >= ChunkY) return;

        ChunkKey ck = WorldToChunk(wx, wz);
        int lx, ly, lz;
        WorldToLocal(wx, wy, wz, lx, ly, lz);

        auto ch = chunks_.GetOrCreate(ck);
        ch->Set(lx, ly, lz, id);
        MarkNeighborsDirtyIfEdge(ck, lx, lz);

        // Optional: wenn Mesh-Worker läuft, gleich meshen
        // EnqueueMesh(ch);
    }

    void World::EnqueueGenerate(const std::shared_ptr<Chunk>& ch) {
        if (!generator_) return;

        // Nur wenn leer/noch nicht generiert
        if (ch->State() != ChunkState::Empty) return;

        ch->SetState(ChunkState::Generating);
        genQ_.Enqueue([this, ch] {
            {
                std::scoped_lock lk(ch->Mutex());
                generator_->Generate(*ch);
            }
            ch->SetState(ChunkState::ReadyData);
            ch->MarkDirtyMesh();
            //EnqueueMesh(ch);
            });
    }

    void World::EnqueueMesh(const std::shared_ptr<Chunk>& ch) {
        // Minimal: kein echtes Meshing, nur State-Übergang + Platzhalter
        if (ch->State() == ChunkState::Meshing) return;

        if (!ch->ConsumeDirtyMesh()) return;

        ch->SetState(ChunkState::Meshing);
        meshQ_.Enqueue([ch] {
            // TODO: hier später Face-Culling/Greedy-Meshing implementieren
            // aktuell nur "mesh exists" markieren
            {
                std::scoped_lock lk(ch->Mutex());
                ch->Mesh().Clear();
            }
            ch->SetState(ChunkState::ReadyMesh);
            });
    }

    void World::UpdateStreaming(int playerWx, int playerWz, int viewDistanceChunks) {
        const ChunkKey center = WorldToChunk(playerWx, playerWz);

        // Zielmenge der Chunks im Radius
        std::unordered_set<ChunkKey, ChunkKeyHash> wanted;
        wanted.reserve((2 * viewDistanceChunks + 1) * (2 * viewDistanceChunks + 1));

        for (int dz = -viewDistanceChunks; dz <= viewDistanceChunks; ++dz) {
            for (int dx = -viewDistanceChunks; dx <= viewDistanceChunks; ++dx) {
                ChunkKey ck{ center.cx + dx, center.cz + dz };
                wanted.insert(ck);

                auto ch = chunks_.GetOrCreate(ck);
                EnqueueGenerate(ch);
                // meshen passiert nach generate
            }
        }

        // Entladen (sehr simpel): alles, was nicht in wanted ist, entfernen
        // (Später: LRU + Persistenz + Sanftes Unload)
        for (auto& ch : chunks_.SnapshotAll()) {
            if (!ch) continue;
            if (wanted.find(ch->Key()) == wanted.end()) {
                chunks_.Remove(ch->Key());
            }
        }
    }

} // namespace BrickWorlds::Voxel
