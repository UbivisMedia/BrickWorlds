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
