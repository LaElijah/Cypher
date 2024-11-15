#include "../../external/GLAD/glad.h"
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "FileReader.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_opengl3.h"
#include "../../external/imgui/imgui_impl_glfw.h"
#include "GUI.h"
#include "GUIComponent.h"
#include "FrameBuffer.h"
#include <functional>
#include "RenderAPI.h"

void Graphics::Renderer::updateWindow(float width, float height)
{
    Canvas->resizeViewport(width, height);
    Camera->setAspectRatio(width / height);
}

std::shared_ptr<Graphics::ResourceManager> Graphics::Renderer::getResourceManager()
{
    return ResourceManager;
}

std::shared_ptr<Graphics::GLFWCanvas> Graphics::Renderer::getCanvas()
{
    return Canvas;
}


std::shared_ptr<Graphics::Camera> Graphics::Renderer::getCamera()
{
    return Camera;
}

std::shared_ptr<Graphics::GUI> Graphics::Renderer::getGUI()
{
    return GUI;
}









// have different "modes" under some identifier key
// like a string naming it, or ma
// into a map and have that map be keys with values 
// of function objects that each have their own implementation of
// the processing of inputs since a person can only have 1 "mode"
// active at a time (theoretically) and pass in the associated key 
// and press type, then




// Move to canvas
bool toggleWindowPressed = false;

void Graphics::Renderer::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        Canvas->releaseMouse(); 
	GUI->enable();
        Camera->disableCamera(); 
    } 

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !toggleWindowPressed) 
    {
	int width;
	int height;

        glfwGetWindowSize(window, &width, &height);

	    GUI->toggleWindow();
            //Canvas->toggleDecoration(); 
	    if (!GUI->isWindowed())
	    {
	        updateWindow(
		    width, 
		    height);

                Canvas->captureMouse(); 
                GUI->disable();	
                Camera->enableCamera();      
	    }
	    else 
	    {
	          Canvas->releaseMouse(); 
		  GUI->enable();	
		  Camera->disableCamera();
		 
	    }

            toggleWindowPressed = true;	    
    }
    else if ( glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
    {
        toggleWindowPressed = false; 
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) 
        Camera->resetPosition(); 

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
        Camera->processKeyboard(Graphics::Direction::FORWARDS, Canvas->getDeltaTime());

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Camera->processKeyboard(Graphics::Direction::LEFT, Canvas->getDeltaTime());

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)   
        Camera->processKeyboard(Graphics::Direction::BACKWARDS, Canvas->getDeltaTime());

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Camera->processKeyboard(Graphics::Direction::RIGHT, Canvas->getDeltaTime());

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        Camera->processKeyboard(Graphics::Direction::UP, Canvas->getDeltaTime());

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
        Camera->processKeyboard(Graphics::Direction::DOWN, Canvas->getDeltaTime());    
}



