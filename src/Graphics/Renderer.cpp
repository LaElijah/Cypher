
#include "Renderer.h"



#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_opengl3.h"
#include "../../external/imgui/imgui_impl_glfw.h"

void Graphics::Renderer::initializeGUI(GLFWwindow* window)
{
IMGUI_CHECKVERSION();
    ImGui::CreateContext();

   
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(); 
   
}


Graphics::Renderer::Renderer(Graphics::GLCanvas* canvas, Graphics::Camera* camera)
{
    Canvas = canvas;
    Camera = camera;
}


void Graphics::Renderer::drawGUI()
{
    
      if (GUI_ENABLED)
      { 
          ImGui_ImplOpenGL3_NewFrame();
          ImGui_ImplGlfw_NewFrame();
          ImGui::NewFrame();
          ImGui::ShowDemoWindow();

          ImGui::Render();
          ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      } 

}

void Graphics::Renderer::end()
{
   ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  
}

void Graphics::Renderer::enableGUI()
{
    GUI_ENABLED = true;
}


void Graphics::Renderer::disableGUI()
{
    GUI_ENABLED = false;
}

void Graphics::Renderer::processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        Canvas->releaseMouse(); 
	enableGUI();
        Camera->disableCamera(); 
    } 

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) 
        Camera->resetPosition(); 

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) 
      glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
      Camera->processKeyboard(Graphics::Direction::FORWARDS, getDeltaTime());
   
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      Camera->processKeyboard(Graphics::Direction::LEFT, getDeltaTime());

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)   
      Camera->processKeyboard(Graphics::Direction::BACKWARDS, getDeltaTime());

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      Camera->processKeyboard(Graphics::Direction::RIGHT, getDeltaTime());

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      Camera->processKeyboard(Graphics::Direction::UP, getDeltaTime());

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
      Camera->processKeyboard(Graphics::Direction::DOWN, getDeltaTime());    
}



void Graphics::Renderer::updateDeltaTime()
{
   
      currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame; 
      lastFrame = currentFrame;


}

float Graphics::Renderer::getDeltaTime()
{
    return deltaTime;
}
