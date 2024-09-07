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
    GUI_ENABLED = true;
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
}




void Graphics::GUI::drawGUI(Graphics::ResourceManager* resourceManager, Graphics::GLCanvas* canvas, Graphics::Camera* camera, Graphics::FrameBuffer* sceneBuffer)
{ 
	// Change so that it checks if fullscreen isnt enabled and gui enabled
      if (true)
      { 
          ImGui_ImplOpenGL3_NewFrame();
          ImGui_ImplGlfw_NewFrame();
          ImGui::NewFrame();
	  ImGui::Begin("Scene");

	  ImVec2 windowSize = ImGui::GetContentRegionAvail();
          const float window_width = windowSize.x;
          const float window_height = windowSize.y; 
	

          // Reset render specs to content region	
          resourceManager->rescaleFBO(window_width, window_height);
  	  canvas->resizeCanvas(window_width, window_height);
  	  camera->setAspectRatio(window_width / window_height);

  	  //ImVec2 pos = ImGui::GetCursorScreenPos(); 
  	  ImGui::Image(
			(ImTextureID)sceneBuffer->getFrameTexture(),
			ImGui::GetContentRegionAvail(),
			ImVec2(0, 1),
			ImVec2(1, 0)
		);

	  ImGui::End();
          ImGui::Render();

          ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
     
	  // Set the draw target buffer after any changes to content are updated and
	  // screen is rendered with scene buffer 
          sceneBuffer->RescaleFrameBuffer(window_width, window_height);
      } 
}




void Graphics::GUI::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(); 
}


