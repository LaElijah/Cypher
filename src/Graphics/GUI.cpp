#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_opengl3.h"
#include "../../external/imgui/imgui_impl_glfw.h"
#include "../../external/imgui/imgui_internal.h"
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
   
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
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
              for (std::pair<std::string, Graphics::GUIComponent*> component : EditorComponents)
	      {
	          component.second->draw();
	          // Add if focused handle input 
	      }
          }
	  
	  if (GUI_ENABLED && !WINDOWED)
	  {

              for (std::pair<std::string, Graphics::GUIComponent*> component : Components)
	      {
	          component.second->draw();
	          // Add if focused handle input 
	      }
	  }

	  ImGui::Render();
          ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	  handleInputs();



     
	  // Set the draw target buffer after any changes to content are updated and
	  // screen is rendered with scene buffer 
}

bool Graphics::isWithinWindow(ImVec2 mousePos, ImVec2 windowPos, ImVec2 windowSize)
{
	
    return (mousePos.x >= windowPos.x 
            && mousePos.x < windowPos.x + windowSize.x 
            && mousePos.y >= windowPos.y 
            && mousePos.y < windowPos.y + windowSize.y);
}

bool Graphics::isWithinContentRegion(ImVec2 mousePos, ImVec2 contentRegionMin, ImVec2 contentRegionMax)
{
	std::cout << "Mouse Position: (" << mousePos.x << ", " << mousePos.y << ")\n";
std::cout << "Content Region Min: (" << contentRegionMin.x << ", " << contentRegionMin.y << ")\n";
std::cout << "Content Region Max: (" << contentRegionMax.x << ", " << contentRegionMax.y << ")\n";

	std::cout << (mousePos.x >= contentRegionMin.x) << std::endl;
	std::cout << (mousePos.y >= contentRegionMin.y) << std::endl;
	std::cout << (mousePos.x <= contentRegionMax.x) << std::endl;
	std::cout << (mousePos.y <= contentRegionMax.y) << std::endl;
    
	return (mousePos.x >= contentRegionMin.x 
            && mousePos.x <= contentRegionMax.x 
	    && mousePos.y >= contentRegionMin.y 
	    && mousePos.y <= contentRegionMax.y);
}



void Graphics::GUI::handleInputs()
{
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
	    ImVec2 mousePos = ImGui::GetMousePos();

        for (auto it = ImGui::GetCurrentContext()->Windows.end(); it != ImGui::GetCurrentContext()->Windows.begin();)
        {
		--it;

		ImGuiWindow* window = *it;

		ImVec2 windowPos = window->Pos;
 
	    if (isWithinWindow(mousePos, windowPos, window->Size))
	    {
                // Mouse is over this window
                std::cout << "Mouse click detected in window: " << window->Name << std::endl;

                // Optionally, check if click is within content region
                // Put this in a free function all 
		
	        ImVec2 contentRegionMin = ImVec2
			        ( 	
				    window->ContentRegionRect.Min.x,
				    window->ContentRegionRect.Min.y
				);

	        ImVec2 contentRegionMax = ImVec2(window->ContentRegionRect.Max.x,
				window->ContentRegionRect.Max.y);	
	
	ImVec2 windowSize = window->Size;
                std::cout << "Window Position: (" << windowPos.x << ", " << windowPos.y << ")\n";
                std::cout << "Window Size: (" << windowSize.x << ", " << windowSize.y << ")\n";	
	
	        ImVec2 localMousePos = ImVec2(mousePos.x - windowPos.x,   mousePos.y - windowPos.y);
		if (isWithinContentRegion(mousePos, contentRegionMin, contentRegionMax)) 
		{
                    
                    std::cout << "Mouse click detected in Region: " << window->Name << std::endl;
                }

                // Found the topmost window that the click was within
                return;
      
            }
	}

    }
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
    EditorComponents[component->getName()] = component;
}

void Graphics::GUI::addComponent(Graphics::GUIComponent* component)
{
    Components[component->getName()] = component;
}
