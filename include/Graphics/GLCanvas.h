#ifndef GLCANVAS_H
#define GLCANVAS_H




#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>
#include <glm/glm.hpp>


namespace Graphics {

  void framebuffer_size_callback(GLFWwindow *window, int width, int height);
  void processInput(GLFWwindow *window, float &visibility, glm::vec3 &a, glm::vec3 &b, glm::vec3 &c, float d);

}


namespace Graphics
{

  class GLCanvas 
{
    // glfw: initialize and configure
    // ------------------------------
    public:
     GLCanvas(unsigned int width = 800, unsigned int height = 600, unsigned int version = 3);
     void initializeCanvas();
     GLFWwindow *getWindow(); 
     void mouse_callback(GLFWwindow *window, double xpos, double ypos);
   

    private:
     int ScreenWidth;
     int ScreenHeight;
     const char *WindowName;
     int VERSION;
     GLFWwindow *Window;
 
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
