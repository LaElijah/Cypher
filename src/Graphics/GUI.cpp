#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_opengl3.h"
#include "../../external/imgui/imgui_impl_glfw.h"
#include "GUI.h"
#include "ResourceManager.h"
#include "GLCanvas.h"
#include "Camera.h"


Graphics::GUI::GUI()
{
    
}


Graphics::GUI::GUI(GLFWwindow* window)
{
    initialize(window);
}



void Graphics::GUI::disable()
{
    GUI_ENABLED = false;
    
}




void Graphics::GUI::enable()
{
    GUI_ENABLED = true;
}




ImGuiIO& Graphics::GUI::getIO()
{
    return ImGui::GetIO();
}

bool Graphics::GUI::isEnabled()
{
    return GUI_ENABLED;
}


void Graphics::GUI::initialize(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext(); 
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(); 
    //ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
}




void Graphics::GUI::drawGUI()
{ 
	// Change so that it checks if fullscreen isnt enabled and gui enabled
          ImGui_ImplOpenGL3_NewFrame();
          ImGui_ImplGlfw_NewFrame();
          ImGui::NewFrame();

	  // Seperate into full screen components vs 
	  // editor components
          if (WINDOWED)
	  {
	      for (GUIComponent* component : EditorComponents)
	      {
	          component->draw();
	          // Add if focused handle input 
	      }
          }
	  
	  if (GUI_ENABLED && !WINDOWED)
	  {
	      for (GUIComponent* component : Components)
	      {
	          component->draw();
	          // Add if focused handle input 
	      }
	  }

	  ImGui::Render();
          ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
     
	  // Set the draw target buffer after any changes to content are updated and
	  // screen is rendered with scene buffer 
}


/*
void Graphics::GUI::render()
{

}
*/

void Graphics::GUI::toggleWindow()
{
    WINDOWED = !WINDOWED;
}


bool Graphics::GUI::isWindowed()
{
    return WINDOWED;
}
void Graphics::GUI::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(); 
}

void Graphics::GUI::addEditorComponent(Graphics::GUIComponent* component)
{
    EditorComponents.push_back(component);
}

void Graphics::GUI::addComponent(Graphics::GUIComponent* component)
{
    Components.push_back(component);
}
