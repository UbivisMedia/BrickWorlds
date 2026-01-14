#include <BrickWorlds/Version.h>
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "BrickWorlds Server v" << BrickWorlds::Version::GetVersionString() << std::endl;
    std::cout << "Starting server..." << std::endl;
    
    // TODO: Initialize server
    
    return 0;
}
