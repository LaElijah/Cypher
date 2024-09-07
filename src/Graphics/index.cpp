#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../../external/imgui/imgui.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "GLCanvas.h"
#include "Renderer.h"

void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);  
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

Graphics::Renderer renderer;
Graphics::ResourceManager* ResourceManager = renderer.getResourceManager();
Graphics::Camera* Camera = renderer.getCamera();
Graphics::GLCanvas* Canvas = renderer.getCanvas();    
Graphics::GUI* GUI = renderer.getGUI();


int main()
{
    GLFWwindow* window = Canvas->getWindow();  

    glfwSetCursorPosCallback(window,  mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback); 

    GUI->initialize(window); 
    
    renderer.run();
    return 0;
}










void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGuiIO& io = GUI->getIO();
    //if (renderer.isFullScreen())
    //{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !io.WantCaptureMouse)
    {
       //Inverse 
	    Canvas->releaseMouse(); 
        GUI->disable();	
        Camera->enableCamera(); 
    }
}


void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{ 
    ImGuiIO& io = GUI->getIO();
    
    if (io.WantCaptureMouse)
    {
	if (Camera->isFirstMouse())
        {
            Camera->setLastX(xpos);
            Camera->setLastY(ypos);
            Camera->startMouse(); 
        }
          
	if (Camera->getCameraStatus())
	{
              Camera->processMousePosition(xpos - Camera->getLastX(), Camera->getLastY() - ypos); 

              Camera->setLastX(xpos);
              Camera->setLastY(ypos);
        }    
    }
}


void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    ImGuiIO& io = GUI->getIO();
    if (!io.WantCaptureMouse)
        Camera->processMouseScroll(yoffset);
}


