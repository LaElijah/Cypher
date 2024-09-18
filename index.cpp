#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void checkFunctionPointer(const char* funcName, void* funcPtr) {
    if (funcPtr) {
        std::cout << funcName << " is available." << std::endl;
    } else {
        std::cerr << funcName << " is not available." << std::endl;
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Test", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Check OpenGL version
    const GLubyte* glVersion = glGetString(GL_VERSION);
    std::cout << "OpenGL version: " << glVersion << std::endl;

    // Check if GL_ARB_multi_draw_indirect is supported
    if (GLAD_GL_ARB_multi_draw_indirect) {
        std::cout << "GL_ARB_multi_draw_indirect is supported" << std::endl;
        checkFunctionPointer("glMultiDrawElementsIndirect", (void*)glMultiDrawElementsIndirect);
    } else {
        std::cerr << "GL_ARB_multi_draw_indirect is not supported" << std::endl;
    }

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

