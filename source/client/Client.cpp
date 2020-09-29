#include "Client.h"

#include <iostream>

Client::Client() {
    // Initalize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create Window
    window = glfwCreateWindow(800, 600, "Tukxel++", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        isAlive = false;
        return;
    }
    glfwMakeContextCurrent(window);

    // Initalize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        isAlive = false;
        return;
    }

    // Initalize Window
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(
        window, [](GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
        });
}

Client::~Client() {
    // Destroy all GLFW stuff
    glfwTerminate();
}

void Client::Render() {
    // Display changes
    glfwSwapBuffers(window);
    glfwPollEvents();
}