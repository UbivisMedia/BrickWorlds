#include <BrickWorlds/Version.h>
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "BrickWorlds Master Server v" << BrickWorlds::Version::GetVersionString() << std::endl;
    std::cout << "Starting master server..." << std::endl;
    
    // TODO: Initialize master server
    
    return 0;
}
