#ifndef GLCANVAS_H
#define GLCANVAS_H




#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>

namespace Graphics
{

  void framebuffer_size_callback(GLFWwindow* window, int width, int height);
  void processInput(GLFWwindow* window);


  class GLCanvas 
{
    // glfw: initialize and configure
    // ------------------------------
    public:
     void initializeCanvas();
     GLFWwindow *getWindow();
   
    private:
     int ScreenWidth;
     int ScreenHeight;
     const char* WindowName;
     int VERSION;
     GLFWwindow* Window;

  
     void startWindow(); 
     void loadGlad();
    
  };


}

#endif
