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
    ResourceManager->initialize(); 
    
    renderer.run();
    return 0;
}



bool pressed = false;
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGuiIO& io = GUI->getIO();

    // If not fullscreen, drag and orient function
    // TODO: FIX error by specifiying the scene content window is focused?
    // else do nothing 
    if (GUI->isWindowed() && !pressed 
		    && button == GLFW_MOUSE_BUTTON_LEFT 
		    && action == GLFW_PRESS 
		    && io.WantCaptureMouse)
    {
	Canvas->captureMouse(); 
        GUI->disable();	
        Camera->enableCamera();
        pressed = true;	
    }

    // If fullscreen, move like camera follow mouse
   
    else if (!GUI->isWindowed() && !pressed && !io.WantCaptureMouse
		    && button == GLFW_MOUSE_BUTTON_LEFT 
		    && action == GLFW_PRESS)
    {
        Canvas->captureMouse(); 
        GUI->disable();	
        Camera->enableCamera();
        pressed = true;	

    } 


    if (
           button == GLFW_MOUSE_BUTTON_LEFT 
           && action == GLFW_RELEASE)
    {
        pressed = false;    
    }

}


void mouse_callback(GLFWwindow *window, double xpos, double ypos)
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


void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    Camera->processMouseScroll(yoffset);
}



