#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../../external/imgui/imgui.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "GLFWCanvas.h"
#include "Renderer.h"
#include "RenderAPI.h"


void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);  
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

Graphics::Renderer renderer;
Graphics::ResourceManager* ResourceManager = renderer.getResourceManager();
Graphics::Camera* Camera = renderer.getCamera();
Graphics::GLFWCanvas* Canvas = renderer.getCanvas();    
Graphics::GUI* GUI = renderer.getGUI();


int main()
{
    GLFWwindow* window = Canvas->getWindow();  

    glfwSetCursorPosCallback(window,  mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback); 

    GUI->initialize(window);
    ResourceManager->initialize(); 
   
    Graphics::OpenGLRenderAPI api = Graphics::OpenGLRenderAPI(); 
    api.loadShaders(ResourceManager->getShaderInfo());
    renderer.run(api);
    return 0;
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGuiIO& io = ImGui::GetIO();
    if (button == GLFW_MOUSE_BUTTON_LEFT 
		    && action == GLFW_PRESS 
		    && !io.WantCaptureMouse
		    && !GUI->isWindowed())
    {
        Canvas->captureMouse(); 
        GUI->disable();	
        Camera->enableCamera(); 
    } 
}



void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{     
    if (Camera->isFirstMouse())
    {
        Canvas->setLastX(xpos);
        Canvas->setLastY(ypos);
  	
	Camera->startMouse(); 
   
    }
      
    if (Camera->getCameraStatus())
    {
          Camera->processMousePosition(xpos - Canvas->getLastX(), Canvas->getLastY() - ypos); 
          Canvas->setLastX(xpos);
          Canvas->setLastY(ypos);
    }
    else 
    {
        glfwSetCursorPos(Canvas->getWindow(), Canvas->getLastX(), Canvas->getLastY()); 
    }    
}


void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    Camera->processMouseScroll(yoffset);
}



