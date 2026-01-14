#include <BrickWorlds/Version.h>
#include <BrickWorlds/Core/World.h>
#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Simple OpenGL headers (platform-specific)
#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#elif defined(__linux__)
#include <GL/gl.h>
#endif

using namespace BrickWorlds::Core;

static void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "BrickWorlds Client v" << BrickWorlds::Version::GetVersionString() << std::endl;
    std::cout << "Initializing rendering system..." << std::endl;
    
    // Initialize GLFW
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }
    
    // Create window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "BrickWorlds - Phase 1 MVP", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    // Create world
    World world;
    std::cout << "Generating flat test world..." << std::endl;
    world.generateFlatWorld(4, 4);
    std::cout << "World generated with " << world.getChunks().size() << " chunks" << std::endl;
    
    // Simple stats
    int totalBlocks = 0;
    for (const auto& pair : world.getChunks()) {
        totalBlocks += CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT;
    }
    std::cout << "Total blocks: " << totalBlocks << std::endl;
    
    // Main loop
    std::cout << "\nRendering window opened! Close window to exit." << std::endl;
    std::cout << "(Note: Full 3D rendering will be added in next iteration)" << std::endl;
    
    while (!glfwWindowShouldClose(window)) {
        // Simple background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    std::cout << "\nShutdown complete." << std::endl;
    return 0;
}
