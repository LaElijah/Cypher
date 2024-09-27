#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdexcept>
#include <iostream>
#include "GLFWCanvas.h"
#include <functional>

namespace Graphics {

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    { 
	//std::cout << width << " " << height << std::endl;
        //glViewport(0, 0, width, height);
    }
}
	
Graphics::GLFWCanvas::GLFWCanvas(
        unsigned int width,
        unsigned int height, 
        unsigned int version) // TODO: Set an enum up for all available versions
{
    VERSION = version;
    lastX = width / 2;
    lastY = height / 2; 
    Width = width;
    Height = height;
    
    startWindow(width, height);
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
    glfwMakeContextCurrent(Window);
    glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback); 

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


GLFWwindow* Graphics::GLFWCanvas::getWindow() 
{
    return Window;
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
    GLFWwindow* window = getWindow();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


void Graphics::GLFWCanvas::captureMouse()
{	
    GLFWwindow* window = getWindow();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(Window, lastX, lastY); 
}	


void Graphics::GLFWCanvas::startWindow(unsigned int& width, unsigned int& height) 
{
  
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, fullscreen);      


    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    Window = glfwCreateWindow(width, height, WindowName.c_str(), NULL, NULL);
    if (Window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw std::runtime_error("Window creation error");
    }

    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


void Graphics::GLFWCanvas::loadGlad() 
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        throw std::runtime_error("GLAD ERROR");
    }
}


void Graphics::GLFWCanvas::resizeViewport(unsigned int width, unsigned height)
{  
    glViewport(0, 0, width, height);
}





