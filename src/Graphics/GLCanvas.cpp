#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdexcept>
#include <iostream>
#include "GLCanvas.h"

namespace Graphics {

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    { 
	std::cout << width << std::endl;
        glViewport(0, 0, width, height);
    }


}

namespace Graphics {    
    
	
	
	
	GLCanvas::GLCanvas(
        unsigned int width,
        unsigned int height, 
        unsigned int version) // TODO: Set an enum up for all available versions
    {
        ScreenWidth = width;
        ScreenHeight = height;
        VERSION = version;
        lastX = width / 2;
        lastY = height / 2;
        initializeCanvas();
    }




    void GLCanvas::initializeCanvas()
    {
        GLCanvas::startWindow();
        glfwMakeContextCurrent(Window);
        // TODO: Move this outside so that i can set canvas screen width and height to correct size
        glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback); 
        GLCanvas::loadGlad(); 
    }




    GLFWwindow *GLCanvas::getWindow() 
    {
        return Window;
    }




    void GLCanvas::releaseMouse()
    {
        GLFWwindow* window = getWindow();
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  



    void GLCanvas::captureMouse()
    {	
	GLFWwindow* window = getWindow();
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }	




    void GLCanvas::startWindow() 
    {
      
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VERSION);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VERSION);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DECORATED, fullscreen);      


        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        Window = glfwCreateWindow(ScreenWidth, ScreenHeight, "Tester", NULL, NULL);
        if (Window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            throw std::runtime_error("Window creation error");
        }

        glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }




    void GLCanvas::loadGlad() 
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            throw std::runtime_error("GLAD ERROR");
        }
    }
};


