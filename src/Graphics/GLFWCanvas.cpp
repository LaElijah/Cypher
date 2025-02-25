#include "../../external/GLAD/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdexcept>
#include <iostream>
#include "GLFWCanvas.h"
#include <functional>

Graphics::GLFWCanvas::GLFWCanvas(
    std::pair<unsigned int, unsigned int> resolution,
    std::shared_ptr<Graphics::Camera> camera,
    std::shared_ptr<Graphics::GUI> gui,
    unsigned int version) // TODO: Set an enum up for all available versions
    : Camera(camera),
      GUI(gui),
      VERSION(version)
{
    lastX = resolution.first / 2;
    lastY = resolution.second / 2;
    Width = resolution.first;
    Height = resolution.second;

    startWindow(resolution.first, resolution.second);
    initialize();
}

void Graphics::GLFWCanvas::updateDeltaTime()
{
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

float Graphics::GLFWCanvas::getDeltaTime()
{
    return deltaTime;
}

void Graphics::GLFWCanvas::setResolution(float width, float height)
{
    Width = width;
    Height = height;
}

void Graphics::GLFWCanvas::initialize()
{
    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, this);

    glfwSetFramebufferSizeCallback(m_Window, frameBufferSizeCallback);
    glfwSetCursorPosCallback(m_Window, mouseCallback);
    glfwSetMouseButtonCallback(m_Window, mouseButtonCallback);
    glfwSetScrollCallback(m_Window, scrollCallback);

    loadGlad();

    glEnable(GL_DEPTH_TEST);
}

float Graphics::GLFWCanvas::getHeight()
{
    return Height;
}

float Graphics::GLFWCanvas::getWidth()
{
    return Width;
}

GLFWwindow *Graphics::GLFWCanvas::getWindow()
{
    return m_Window;
}

float Graphics::GLFWCanvas::getLastX()
{
    return lastX;
}

float Graphics::GLFWCanvas::getLastY()
{
    return lastY;
}

void Graphics::GLFWCanvas::setLastX(double X)
{
    lastX = X;
}

void Graphics::GLFWCanvas::setLastY(double Y)
{
    lastY = Y;
}

void Graphics::GLFWCanvas::releaseMouse()
{
    GLFWwindow *window = getWindow();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Graphics::GLFWCanvas::captureMouse()
{
    GLFWwindow *window = getWindow();
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(m_Window, lastX, lastY);
}

void Graphics::GLFWCanvas::startWindow(unsigned int &width, unsigned int &height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, fullscreen);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_Window = glfwCreateWindow(width, height, WindowName.c_str(), NULL, NULL);
    if (m_Window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw std::runtime_error("Window creation error");
    }

    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char *message,
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        std::cout << "Source: API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        std::cout << "Source: Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        std::cout << "Source: Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        std::cout << "Source: Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        std::cout << "Source: Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        std::cout << "Source: Other";
        break;
    }
    std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "Type: Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "Type: Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "Type: Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "Type: Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "Type: Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        std::cout << "Type: Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        std::cout << "Type: Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        std::cout << "Type: Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "Type: Other";
        break;
    }
    std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "Severity: high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "Severity: medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "Severity: low";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::cout << "Severity: notification";
        break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void Graphics::GLFWCanvas::loadGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        throw std::runtime_error("GLAD ERROR");
    }

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        // initialize debug output
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        //glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
}

void Graphics::GLFWCanvas::resizeViewport(unsigned int width, unsigned height)
{
    glViewport(0, 0, width, height);
}

void Graphics::GLFWCanvas::mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    static_cast<GLFWCanvas *>(glfwGetWindowUserPointer(window))
        ->mouseCallbackImpl(window, xpos, ypos);
}

void Graphics::GLFWCanvas::mouseCallbackImpl(GLFWwindow *window, double xpos, double ypos)
{
    if (Camera->isFirstMouse())
    {
        setLastX(xpos);
        setLastY(ypos);
        Camera->startMouse();
    }

    if (Camera->getCameraStatus())
    {
        Camera->processMousePosition(xpos - getLastX(), getLastY() - ypos);
        setLastX(xpos);
        setLastY(ypos);
    }
    else
    {
        glfwSetCursorPos(getWindow(), getLastX(), getLastY());
    }
}

void Graphics::GLFWCanvas::mouseButtonCallbackImpl(GLFWwindow *window, int button, int action, int mods)
{
    ImGuiIO &io = ImGui::GetIO();
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !io.WantCaptureMouse && !GUI->isWindowed())
    {
        captureMouse();
        GUI->disable();
        Camera->enableCamera();
    }
}

void Graphics::GLFWCanvas::scrollCallbackImpl(GLFWwindow *window, double xoffset, double yoffset)
{
    Camera->processMouseScroll(yoffset);
}

void Graphics::GLFWCanvas::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    static_cast<GLFWCanvas *>(glfwGetWindowUserPointer(window))
        ->mouseButtonCallbackImpl(window, button, action, mods);
}

void Graphics::GLFWCanvas::scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    static_cast<GLFWCanvas *>(glfwGetWindowUserPointer(window))
        ->scrollCallbackImpl(window, xoffset, yoffset);
}

void Graphics::GLFWCanvas::frameBufferSizeCallbackImpl(GLFWwindow *window, int width, int height)
{
    // std::cout << width << " " << height << std::endl;
    // glViewport(0, 0, width, height);
}

void Graphics::GLFWCanvas::frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{

    static_cast<GLFWCanvas *>(glfwGetWindowUserPointer(window))
        ->frameBufferSizeCallbackImpl(window, width, height);
}
