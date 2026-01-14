#include <BrickWorlds/Version.h>
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "BrickWorlds Client v" << BrickWorlds::Version::GetVersionString() << std::endl;
    std::cout << "Starting client..." << std::endl;
    
    // TODO: Initialize client
    
    return 0;
}
