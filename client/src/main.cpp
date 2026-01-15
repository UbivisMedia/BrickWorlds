#include <BrickWorlds/Version.h>
#include <BrickWorlds/Voxel/World.h>
#include <BrickWorlds/Voxel/FlatGenerator.h>
#include <BrickWorlds/Voxel/BlockId.h>
#include "Camera.h"
#include "Renderer.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace BrickWorlds::Voxel;

struct InputState {
    bool keys[512] = { false };
    double lastMouseX = 640.0;
    double lastMouseY = 360.0;
    bool firstMouse = true;
    float mouseSensitivity = 0.05f;  // <- Von 0.1f auf 0.05f reduziert
} g_input;


static void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int, int action, int) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
    if (key >= 0 && key < 512) {
        if (action == GLFW_PRESS) g_input.keys[key] = true;
        else if (action == GLFW_RELEASE) g_input.keys[key] = false;
    }
}

static void mouse_callback(GLFWwindow*, double xpos, double ypos) {
    if (g_input.firstMouse) {
        g_input.lastMouseX = xpos;
        g_input.lastMouseY = ypos;
        g_input.firstMouse = false;
        return;
    }

    double xoffset = xpos - g_input.lastMouseX;
    double yoffset = g_input.lastMouseY - ypos; // Reversed

    g_input.lastMouseX = xpos;
    g_input.lastMouseY = ypos;

}



void processInput(Camera& camera, float deltaTime) {
    float moveSpeed = 15.0f * deltaTime;  // Erhöht von 10.0f auf 15.0f
    bool moved = false;

    if (g_input.keys[GLFW_KEY_W]) {
        camera.move(camera.getForward() * moveSpeed);
        moved = true;
    }
    if (g_input.keys[GLFW_KEY_S]) {
        camera.move(camera.getForward() * (-moveSpeed));
        moved = true;
    }
    if (g_input.keys[GLFW_KEY_A]) {
        camera.move(camera.getRight() * (-moveSpeed));
        moved = true;
    }
    if (g_input.keys[GLFW_KEY_D]) {
        camera.move(camera.getRight() * moveSpeed);
        moved = true;
    }
    if (g_input.keys[GLFW_KEY_SPACE]) {
        camera.move(BrickWorlds::Core::Vector3f(0.0f, moveSpeed, 0.0f));
        moved = true;
    }
    if (g_input.keys[GLFW_KEY_LEFT_SHIFT]) {
        camera.move(BrickWorlds::Core::Vector3f(0.0f, -moveSpeed, 0.0f));
        moved = true;
    }

    if (moved) {
        auto pos = camera.getPosition();
        std::cout << "Camera pos: (" << pos.x << ", " << pos.y << ", " << pos.z << ")\r" << std::flush;
    }
}


int main(int argc, char* argv[]) {
    std::cout << "BrickWorlds Client v" << BrickWorlds::Version::GetVersionString() << std::endl;
    std::cout << "Initializing 3D rendering system..." << std::endl;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "BrickWorlds - Phase 1 MVP (3D)", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewError) << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    FlatGenerator generator;
    World world(&generator);

    // 1 Gen-Thread, 1 Mesh-Thread (Mesh ist zunächst Stub/CPU-Mesh, je nachdem wie du es umgesetzt hast)
    world.StartStreaming(1, 1);

    std::cout << "Starting chunk streaming..." << std::endl;


    Renderer renderer;
    if (!renderer.initialize()) {
        std::cerr << "Failed to initialize renderer!" << std::endl;
        glfwTerminate();
        return -1;
    }

    Camera camera(70.0f, 1280.0f / 720.0f);
    camera.setPosition(BrickWorlds::Core::Vector3f(32.0f, 20.0f, 50.0f));
    camera.setRotation(0.0f, 180.0f);  // Geradeaus schauen

    std::cout << "\nControls:" << std::endl;
    std::cout << "  WASD - Move" << std::endl;
    std::cout << "  Mouse - Look around" << std::endl;
    std::cout << "  Space - Up, Shift - Down" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    std::cout << "\nRendering...\n" << std::endl;

    // Lambda mit Kamera-Capture
    auto mouseCallback = [&camera](GLFWwindow*, double xpos, double ypos) {
        if (g_input.firstMouse) {
            g_input.lastMouseX = xpos;
            g_input.lastMouseY = ypos;
            g_input.firstMouse = false;
            return;
        }

        double xoffset = xpos - g_input.lastMouseX;
        double yoffset = g_input.lastMouseY - ypos;

        g_input.lastMouseX = xpos;
        g_input.lastMouseY = ypos;

        // Direkt rotieren
        camera.rotate(
            static_cast<float>(yoffset * g_input.mouseSensitivity),
            static_cast<float>(xoffset * g_input.mouseSensitivity)
        );
        };

    // Callback setzen mit std::function Wrapper
    // Für C-API müssen wir einen Workaround machen
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double xpos, double ypos) {
        Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(win));

        if (g_input.firstMouse) {
            g_input.lastMouseX = xpos;
            g_input.lastMouseY = ypos;
            g_input.firstMouse = false;
            return;
        }

        double xoffset = xpos - g_input.lastMouseX;
        double yoffset = g_input.lastMouseY - ypos;

        g_input.lastMouseX = xpos;
        g_input.lastMouseY = ypos;

        cam->rotate(
            static_cast<float>(yoffset * g_input.mouseSensitivity),
            static_cast<float>(xoffset * g_input.mouseSensitivity)
        );
        });

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        processInput(camera, deltaTime);
        auto pos = camera.getPosition();
        int playerWx = static_cast<int>(pos.x);
        int playerWz = static_cast<int>(pos.z);

        // viewDistanceChunks: erstmal klein starten (z.B. 4-6)
        world.UpdateStreaming(playerWx, playerWz, 6);

        renderer.render(world, camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }




    world.StopStreaming();
    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "\nShutdown complete." << std::endl;
    return 0;
}
