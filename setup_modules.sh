#!/bin/bash
# Setup script to create remaining module files for BrickWorlds project

echo "Creating BrickWorlds project modules..."

# Create client module
echo "Creating client module..."
mkdir -p client/src

cat > client/CMakeLists.txt << 'EOF'
project(BrickWorlds_Client)

file(GLOB_RECURSE CLIENT_SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${CLIENT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE BrickWorlds_Shared)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    OUTPUT_NAME "BrickWorlds_Client"
)

message(STATUS "Configured Client executable")
EOF

cat > client/src/main.cpp << 'EOF'
#include <BrickWorlds/Version.h>
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "BrickWorlds Client v" << BrickWorlds::Version::GetVersionString() << std::endl;
    std::cout << "Starting client..." << std::endl;
    
    // TODO: Initialize client
    
    return 0;
}
EOF

# Create server module
echo "Creating server module..."
mkdir -p server/src

cat > server/CMakeLists.txt << 'EOF'
project(BrickWorlds_Server)

file(GLOB_RECURSE SERVER_SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SERVER_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE BrickWorlds_Shared)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    OUTPUT_NAME "BrickWorlds_Server"
)

message(STATUS "Configured Server executable")
EOF

cat > server/src/main.cpp << 'EOF'
#include <BrickWorlds/Version.h>
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "BrickWorlds Server v" << BrickWorlds::Version::GetVersionString() << std::endl;
    std::cout << "Starting server..." << std::endl;
    
    // TODO: Initialize server
    
    return 0;
}
EOF

# Create master module
echo "Creating master module..."
mkdir -p master/src

cat > master/CMakeLists.txt << 'EOF'
project(BrickWorlds_Master)

file(GLOB_RECURSE MASTER_SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${MASTER_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE BrickWorlds_Shared)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    OUTPUT_NAME "BrickWorlds_Master"
)

message(STATUS "Configured Master executable")
EOF

cat > master/src/main.cpp << 'EOF'
#include <BrickWorlds/Version.h>
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "BrickWorlds Master Server v" << BrickWorlds::Version::GetVersionString() << std::endl;
    std::cout << "Starting master server..." << std::endl;
    
    // TODO: Initialize master server
    
    return 0;
}
EOF

echo ""
echo "✓ All modules created successfully!"
echo ""
echo "Next steps:"
echo "  1. Run 'cmake -B build' to configure the project"
echo "  2. Run 'cmake --build build --config Release' to compile"
echo "  3. Find binaries in build/bin/"
echo ""
