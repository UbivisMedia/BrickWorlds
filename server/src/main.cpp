#include <BrickWorlds/Version.h>
#include <BrickWorlds/Voxel/World.h>
#include <BrickWorlds/Voxel/FlatGenerator.h>

#include <chrono>
#include <iostream>
#include <thread>

int main(int argc, char* argv[]) {
    std::cout << "BrickWorlds Server v" << BrickWorlds::Version::GetVersionString() << std::endl;
    std::cout << "Starting server..." << std::endl;

    using namespace BrickWorlds::Voxel;

    FlatGenerator generator;
    World world(&generator);

    // 1 Gen-Thread, 1 Mesh-Thread (Mesh ist aktuell noch Stub – passt)
    world.StartStreaming(1, 1);

    int playerWx = 0;
    int playerWz = 0;
    const int viewDistanceChunks = 6;

    for (int tick = 0; tick < 300; ++tick) {
        world.UpdateStreaming(playerWx, playerWz, viewDistanceChunks);

        // Debug: Anzahl geladener Chunks
        auto all = world.Chunks().SnapshotAll();
        std::cout << "Tick " << tick << " | loaded chunks: " << all.size() << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    world.StopStreaming();
    std::cout << "Server shutdown." << std::endl;
    return 0;
}
