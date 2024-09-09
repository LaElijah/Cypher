#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdexcept>
#include <iostream>
#include "GLCanvas.h"
#include <functional>


namespace Graphics {


    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    { 
       	
	    std::cout << width << " " << height << std::endl;
        glViewport(0, 0, width, height);
    }


}

namespace Graphics {    
    
	
	
	
	GLCanvas::GLCanvas(
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


    void GLCanvas::toggleDecoration()
    {
         
    }
 
    void GLCanvas::setResolution(float width, float height)
    {
        Width = width;
        Height = height;	
    }
    void GLCanvas::initialize()
    {
        glfwMakeContextCurrent(Window);
        // TODO: Move this outside so that i can set canvas screen width and height to correct size
        

	glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback); 
        GLCanvas::loadGlad();  
        glEnable(GL_DEPTH_TEST);
    }


    float GLCanvas::getHeight()
    {
        return Height; 
    }


    float GLCanvas::getWidth()
    {
        return Width; 
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




    void GLCanvas::startWindow(unsigned int& width, unsigned int& height) 
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




    void GLCanvas::loadGlad() 
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            throw std::runtime_error("GLAD ERROR");
        }
    }


    void GLCanvas::resizeCanvas(unsigned int width, unsigned height)
    {  
        glViewport(0, 0, width, height);
    }



};


