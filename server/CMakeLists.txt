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
