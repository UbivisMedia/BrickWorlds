# Shared library - Common code for all components

project(BrickWorlds_Shared)

# Collect all source files
file(GLOB_RECURSE SHARED_SOURCES
    "src/*.cpp"
)

file(GLOB_RECURSE SHARED_HEADERS
    "include/*.h"
    "include/*.hpp"
)

# Create shared library
add_library(${PROJECT_NAME} STATIC
    ${SHARED_SOURCES}
    ${SHARED_HEADERS}
)

# Include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Set target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

message(STATUS "Configured Shared library")
