#ifndef GLCANVAS_H
#define GLCANVAS_H




#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>
#include <glm/glm.hpp>


namespace Graphics {

  void framebuffer_size_callback(GLFWwindow *window, int width, int height);

}


namespace Graphics
{

  class GLCanvas 
{
    // glfw: initialize and configure
    // ------------------------------
    public:
     GLCanvas(unsigned int width, unsigned int height, unsigned int version = 3);
     void initializeCanvas();
     GLFWwindow* getWindow(); 
     void mouse_callback(GLFWwindow *window, double xpos, double ypos);
     void captureMouse();
     void releaseMouse(); 

    private:
     int ScreenWidth;
     int ScreenHeight;
     const char* WindowName;
     int VERSION;
     GLFWwindow* Window;
     bool fullscreen = false;
 
     float pitch;
     float yaw;

     int lastX;
     int lastY;

     bool firstMouse;

     glm::vec3 cameraFront;
  
     void startWindow(); 
     void loadGlad();
    
  };


}

#endif
