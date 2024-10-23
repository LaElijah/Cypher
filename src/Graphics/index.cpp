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

std::shared_ptr<Graphics::Camera> Camera = std::shared_ptr<Graphics::Camera>(new Graphics::Camera(1920, 1080));
std::shared_ptr<Graphics::GUI> GUI = std::shared_ptr<Graphics::GUI>(new Graphics::GUI());

std::shared_ptr<Graphics::GLFWCanvas> Canvas = std::shared_ptr<Graphics::GLFWCanvas>(
		new Graphics::GLFWCanvas
		            (
				std::make_pair(1920, 1080),
				Camera, 
				GUI
		            )
		);

Graphics::Renderer renderer = Graphics::Renderer(
		                Canvas,
		                Camera,
		                GUI
		);


Graphics::ResourceManager* ResourceManager = renderer.getResourceManager();


int main()
{
    GLFWwindow* window = Canvas->getWindow();  
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



