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
