#include <BrickWorlds/Version.h>
#include <BrickWorlds/Core/World.h>
#include "Camera.h"
#include "Renderer.h"
#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#elif defined(__linux__)
#include <GL/gl.h>
#endif

using namespace BrickWorlds::Core;

// Input state
struct InputState {
    bool keys[512] = { false };
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    bool firstMouse = true;
} g_input;

static void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
    if (key >= 0 && key < 512) {
        if (action == GLFW_PRESS) g_input.keys[key] = true;
        else if (action == GLFW_RELEASE) g_input.keys[key] = false;
    }
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (g_input.firstMouse) {
        g_input.lastMouseX = xpos;
        g_input.lastMouseY = ypos;
        g_input.firstMouse = false;
    }

    double xoffset = xpos - g_input.lastMouseX;
    double yoffset = g_input.lastMouseY - ypos; // Reversed
    g_input.lastMouseX = xpos;
    g_input.lastMouseY = ypos;

    // Apply to camera later in main loop
}

void processInput(Camera& camera, float deltaTime) {
    float moveSpeed = 10.0f * deltaTime;

    if (g_input.keys[GLFW_KEY_W]) {
        camera.move(camera.getForward() * moveSpeed);
    }
    if (g_input.keys[GLFW_KEY_S]) {
        camera.move(camera.getForward() * (-moveSpeed));
    }
    if (g_input.keys[GLFW_KEY_A]) {
        camera.move(camera.getRight() * (-moveSpeed));
    }
    if (g_input.keys[GLFW_KEY_D]) {
        camera.move(camera.getRight() * moveSpeed);
    }
    if (g_input.keys[GLFW_KEY_SPACE]) {
        camera.move(Vector3f(0.0f, moveSpeed, 0.0f));
    }
    if (g_input.keys[GLFW_KEY_LEFT_SHIFT]) {
        camera.move(Vector3f(0.0f, -moveSpeed, 0.0f));
    }
}

int main(int argc, char* argv[]) {
    std::cout << "BrickWorlds Client v" << BrickWorlds::Version::GetVersionString() << std::endl;
    std::cout << "Initializing 3D rendering system..." << std::endl;

    // Initialize GLFW
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    // Create window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "BrickWorlds - Phase 1 MVP (3D)", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Create world
    World world;
    std::cout << "Generating flat test world..." << std::endl;
    world.generateFlatWorld(4, 4);
    std::cout << "World generated with " << world.getChunks().size() << " chunks" << std::endl;

    // Initialize renderer
    Renderer renderer;
    if (!renderer.initialize()) {
        std::cerr << "Failed to initialize renderer!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Create camera
    Camera camera(70.0f, 1280.0f / 720.0f);
    camera.setPosition(Vector3f(32.0f, 15.0f, 32.0f));
    camera.setRotation(0.0f, -45.0f);

    std::cout << "\nControls:" << std::endl;
    std::cout << "  WASD - Move" << std::endl;
    std::cout << "  Mouse - Look around" << std::endl;
    std::cout << "  Space - Up" << std::endl;
    std::cout << "  Shift - Down" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    std::cout << "\nRendering..." << std::endl;

    // Main loop
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        // Process input
        processInput(camera, deltaTime);

        // Update mouse look
        if (!g_input.firstMouse) {
            // Mouse sensitivity is handled in the mouse_callback
        }

        // Render
        renderer.render(world, camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "\nShutdown complete." << std::endl;
    return 0;
}

