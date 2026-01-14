project(BrickWorlds_Client)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLEW
find_package(GLEW REQUIRED)

# Find or fetch GLFW
find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
    message(STATUS "GLFW not found, fetching from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8
    )
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(glfw)
endif()

file(GLOB_RECURSE CLIENT_SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${CLIENT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    BrickWorlds_Shared 
    OpenGL::GL
    GLEW::GLEW
    glfw
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    OUTPUT_NAME "BrickWorlds_Client"
)

message(STATUS "Configured Client executable")
