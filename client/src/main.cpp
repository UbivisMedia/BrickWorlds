#include <BrickWorlds/Version.h>
#include <BrickWorlds/Core/World.h>
#include <iostream>

using namespace BrickWorlds::Core;

int main(int argc, char* argv[]) {
    std::cout << "BrickWorlds Client v" << BrickWorlds::Version::GetVersionString() << std::endl;
    std::cout << "Starting Phase 1 - Lokaler MVP Test..." << std::endl;
    
    // Create world
    World world;
    
    // Generate flat test world
    std::cout << "Generating flat world..." << std::endl;
    world.generateFlatWorld(4, 4);
    
    // Test: Get some blocks
    std::cout << "\nTesting block access:" << std::endl;
    for (int x = 0; x < 5; x++) {
        BlockID block = world.getBlock(x, 0, 0);
        std::cout << "  Block at (" << x << ", 0, 0): ID=" << block << std::endl;
    }
    
    // Test: Set a block
    std::cout << "\nSetting block at (2, 5, 2) to WOOD..." << std::endl;
    world.setBlock(2, 5, 2, BLOCK_WOOD);
    BlockID testBlock = world.getBlock(2, 5, 2);
    std::cout << "  Verification: Block ID=" << testBlock << std::endl;
    
    // Display chunk count
    std::cout << "\nLoaded chunks: " << world.getChunks().size() << std::endl;
    
    std::cout << "\nPhase 1 basic systems functional!" << std::endl;
    std::cout << "Next steps: Add rendering, input, and networking" << std::endl;
    
    return 0;
}
