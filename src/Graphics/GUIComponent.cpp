#include "GUIComponent.h"
#include "../../external/imgui/imgui.h"
#include "../../external/imgui/backends/imgui_impl_opengl3.h"
#include "../../external/imgui/backends/imgui_impl_glfw.h"
#include <cstdint>


Graphics::GUIComponent::GUIComponent(std::string name)
: Name(name)
{}

std::string Graphics::GUIComponent::getName()
{
    return Name;
}

Graphics::TestWindow::TestWindow(std::string name) 
: GUIComponent(name)
{
    Name = name;
}

void Graphics::TestWindow::draw()
{
 
	  ImGui::Begin(Name.c_str());

	  ImGui::Text("HII");

	  ImGui::End();   
}

void Graphics::TestWindow::handleInput()
{
}

Graphics::ModelWindow::ModelWindow
(
    std::string name,
    std::string directory,
    std::function<void(const char* string)> addModel
)
: GUIComponent(name),
  FILE_READER(directory),
  ADD_MODEL(addModel)
{
    Name = name;
    Directory = directory;

};


void Graphics::ModelWindow::draw()
{
    std::vector<std::string> files = FILE_READER.getFiles(); 
    ImGui::Begin(Name.c_str());

    if (ImGui::CollapsingHeader("Loaded Models"))
    {
	for (std::string file : files)
	    if(ImGui::Button(file.c_str()))
        {
            std::cout << "CLICK" << std::endl;
            ADD_MODEL((Directory + "/" + file + "/scene.gltf").c_str());

        }
    }
    ImGui::End();   
}

void Graphics::ModelWindow::handleInput()
{

}

Graphics::SceneWindow::SceneWindow
(
    std::string name,
    Graphics::FrameBuffer* sceneBuffer,
    std::function<void(float, float)>& resizeFunction,
    std::vector<std::function<void()>>& functionStorage
) 
: resizeWindow(resizeFunction),
  postRenderFunctions(functionStorage),
  GUIComponent(name)
{
    Name = name; 
    SceneBuffer = sceneBuffer;
    resizeWindow = resizeFunction; 
    postRenderFunctions = functionStorage;
}
   
void Graphics::SceneWindow::draw()
{
	ImGui::Begin(Name.c_str());

	ImVec2 windowSize = ImGui::GetContentRegionAvail();
        ImGui::Image(
            (ImTextureID)(uintptr_t)(SceneBuffer->getFrameTexture()),
	    windowSize,
	    ImVec2(0, 1),
	    ImVec2(1, 0));


	resizeWindow(windowSize.x, windowSize.y); 
 
	std::function<void()> resizeBufferWindow = [this] () 
        {
            SceneBuffer->RescaleFrameBuffer();
        };

        postRenderFunctions.push_back(resizeBufferWindow);
	 

	ImGui::End();
          

	// I dont want to resize the buffer before im done
	// rendering so I want to defer it to my 
	// post rendering queue, it has no params
	// because i have all the state i need 
	// in each component
}

void Graphics::SceneWindow::handleInput()
{
    std::cout << "CLICKED" << std::endl;
    // Add here the click and drag function
    // and other functions to make outside the gui
}









