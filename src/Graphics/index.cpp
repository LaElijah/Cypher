#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GLCanvas.h"
#include "Shader.h"
#include <glm/ext/matrix_transform.hpp>
#include <math.h>
#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_opengl3.h"
#include "../../external/imgui/imgui_impl_glfw.h"
//#include "../../external/STB_IMAGE/stb_image.h"
#include "../../external/STB_IMAGE/stb_image.h"
//#include "../../include/Graphics/Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Globals.cpp"
#include "Model.h"
#include "ResourceManager.h"
#include <iostream>
#include "FileReader.h"
#include "Renderer.h"



float SCREEN_WIDTH = 1920;
float SCREEN_HEIGHT = 1080;

void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);  
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

Graphics::GLCanvas* canvas = new Graphics::GLCanvas(SCREEN_WIDTH, SCREEN_HEIGHT);    
Graphics::Camera* camera = new Graphics::Camera(SCREEN_WIDTH, SCREEN_HEIGHT);

Graphics::ResourceManager* resourceManager = new Graphics::ResourceManager();

Graphics::Renderer renderer(resourceManager, canvas, camera);


int main()
{
	GLFWwindow* window = canvas->getWindow();
	resourceManager->loadModelPaths();
     
    glEnable(GL_DEPTH_TEST);
    glfwSetCursorPosCallback(window,  mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
 
    renderer.initializeGUI(window); 
    renderer.start();
   return 0;
}



















void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

    ImGuiIO& io = ImGui::GetIO();
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !io.WantCaptureMouse)
    {
        canvas->captureMouse(); 
        renderer.disableGUI();	
        camera->enableCamera(); 
    }
}



void mouse_callback(GLFWwindow *window, double xpos, double ypos)
    {
      
    ImGuiIO& io = ImGui::GetIO();
    
    if (!io.WantCaptureMouse)
    {
	  if (camera->isFirstMouse())
          {
            camera->setLastX(xpos);
            camera->setLastY(ypos);
            camera->startMouse(); 
          }
          
	  if (camera->getCameraStatus())
	  {
              camera->processMousePosition(xpos - camera->getLastX(), camera->getLastY() - ypos); 

              camera->setLastX(xpos);
              camera->setLastY(ypos);
	  }    

    }

    }


void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{

    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse)
        camera->processMouseScroll(yoffset);
}

