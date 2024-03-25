#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
#include <stdexcept>
#include <iostream>
#include "GLCanvas.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/ext/matrix_transform.hpp>

namespace Graphics {

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
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
      glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback); 
      GLCanvas::loadGlad(); 
    }

    GLFWwindow *GLCanvas::getWindow() 
    {
      return Window;
    }

    void GLCanvas::startWindow() 
    {
      
      glfwInit();
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VERSION);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VERSION);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_DECORATED, false);      


    #ifdef __APPLE__
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // glfw window creation
    // --------------------
      Window = glfwCreateWindow(ScreenWidth, ScreenHeight, "Test", NULL, NULL);
      if (Window == NULL)
      {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw std::runtime_error("Window creation error");
      }


      glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    void GLCanvas::loadGlad() 
    {
    // glad: load all OpenGL function pointers
    // ---------------------------------------
      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
      {
        std::cout << "Failed to initialize GLAD" << std::endl;
        throw std::runtime_error("GLAD ERROR");
      }
    }
};


