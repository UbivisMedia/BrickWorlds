project(BrickWorlds_Client)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find or fetch GLEW
find_package(GLEW QUIET)
if(NOT GLEW_FOUND)
    message(STATUS "GLEW not found, fetching from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
        glew
        URL https://github.com/nigels-com/glew/releases/download/glew-2.3.0/glew-2.3.0.zip
    )
    
    set(BUILD_UTILS OFF CACHE BOOL "" FORCE)
    set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "" FORCE)
    
    FetchContent_GetProperties(glew)
    if(NOT glew_POPULATED)
        FetchContent_Populate(glew)
        add_subdirectory(${glew_SOURCE_DIR}/build/cmake ${glew_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
    
    # Create alias
    if(TARGET glew_s)
        add_library(GLEW::GLEW ALIAS glew_s)
    elseif(TARGET libglew_static)
        add_library(GLEW::GLEW ALIAS libglew_static)
    endif()
endif()

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

# Collect source files
file(GLOB_RECURSE CLIENT_SOURCES "src/*.cpp")

# Create executable - ERST JETZT!
add_executable(${PROJECT_NAME} ${CLIENT_SOURCES})

# NOW we can add include directories (target exists!)
if(NOT GLEW_FOUND AND glew_SOURCE_DIR)
    target_include_directories(${PROJECT_NAME} PRIVATE ${glew_SOURCE_DIR}/include)
endif()

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    BrickWorlds_Shared 
    OpenGL::GL
    GLEW::GLEW
    glfw
)

# Set properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    OUTPUT_NAME "BrickWorlds_Client"
)

message(STATUS "Configured Client executable")
