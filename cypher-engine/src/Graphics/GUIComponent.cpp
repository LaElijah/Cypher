#include "GUIComponent.h"
#include "imgui.h"
#include "FileReader.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include <cstdint>
#include <nlohmann/json.hpp>
#include <regex>
#include <set>
#include <glm/gtc/matrix_transform.hpp>
#include <format>
#include "IconsFontAwesome5.h"
#include <cstring> 
#include "Primitives.h"

Graphics::GUIComponent::GUIComponent(std::string name)
    : Name(name)
{
    
}

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
const char* getFileIcon(std::string extension) {
    if (!extension.empty() && extension[0] == '.') {
        extension.erase(0, 1);
    }
    
    std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c) {
        return std::tolower(c);
    });


    // 4. Find and return the icon string macro
    auto it = Graphics::IconMap.find(extension);
    if (it != Graphics::IconMap.end()) {
        return it->second;
    }

    // Default fallback icon for unknown variants
    return ICON_FA_FILE; 
}
int Graphics::FileExplorerWindow::windowCount = 0;
Graphics::FileExplorerWindow::FileExplorerWindow
(
    std::string path,
    std::shared_ptr<char[]> pathResult,
    std::shared_ptr<bool> activeState,
    std::shared_ptr<std::vector<std::filesystem::directory_entry>> files,
    bool directoriesOnly = false
) : 
    GUIComponent("Explorer##" + std::to_string(windowCount)),
    path(path), 
    pathResult(pathResult), 
    activeState(activeState), 
    directoriesOnly(directoriesOnly),
    files(files)
{
    pathResult[0] = '/';
    pathResult[1] = '\0';
    *files = Graphics::FileReader::getDirectoryContents(std::string(pathResult.get()));
    fileNameBuffer = std::make_unique<char[]>(256);
    ++windowCount;
}

void Graphics::FileExplorerWindow::handleInput()
{

}

void Graphics::FileExplorerWindow::draw()
{
    auto windowMin = ImVec2(290.0f, 200.0f);
    auto windowMax = ImVec2(FLT_MAX, FLT_MAX);
    ImGui::SetNextWindowSizeConstraints(windowMin, windowMax);
    ImGui::Begin(Name.c_str());
    if(ImGui::Button
    (
        ICON_FA_ARROW_UP
        //directoryFilter(std::string(directoryBuffer.get()))
    ))
    {
        char* lastSlash = std::strrchr(pathResult.get(), '/');

        if (lastSlash != nullptr)
        {
            if (lastSlash == pathResult.get())
                *(lastSlash + 1) = '\0';
            else
                *lastSlash = '\0';
        }

        *files = Graphics::FileReader::getDirectoryContents(std::string(pathResult.get()));
    }

    ImGui::SameLine(); ImGui::InputText
    (
        ("##" + Name).c_str(), 
        pathResult.get(), 
        256
        //directoryFilter(std::string(directoryBuffer.get()))
    );

    ImGui::SameLine(); 
    if (ImGui::Button(ICON_FA_REDO))
    {
        *files = Graphics::FileReader::getDirectoryContents(std::string(pathResult.get()));
    }

    float buttonWidth = 80.0f;
    float availableWidth = ImGui::GetContentRegionAvail().x;
    float spacingX = ImGui::GetStyle().ItemSpacing.x;
    float spacingY = ImGui::GetStyle().ItemSpacing.y;
    float inputTextWidth = availableWidth - (buttonWidth * 3) - (spacingX * 2);
    bool wrapFooter = false; 
    float frameHeight = ImGui::GetFrameHeight(); 

    if (inputTextWidth < 50.0f)
    {
        inputTextWidth = 50.0f;
        wrapFooter = true;
    }

    inputTextWidth = wrapFooter ? (availableWidth - (buttonWidth - (spacingX / 4))) : (availableWidth - (buttonWidth * 3) - (spacingX * 2));

    float footerHeight = !wrapFooter 
    ? (frameHeight + spacingY * 2) 
    : (frameHeight * 2 + spacingY * 3);
    //float footerHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    if (ImGui::BeginChild("ScrollingContentRegion", ImVec2(0, -footerHeight), ImGuiChildFlags_None))
    {
        
        for (std::filesystem::directory_entry file : *files.get())
        {
            std::string filestring = file.path().generic_string();
            std::string filename = file.path().filename().generic_string();
            std::string label = (file.is_directory() ? std::string(ICON_FA_FOLDER) : std::string(getFileIcon(file.path().extension().string()))) + " " + filename;

            if (file.is_directory() || !directoriesOnly)
                if (ImGui::Selectable(label.c_str(), false))
                {
                    if (file.is_directory())
                    {
                        std::memcpy(pathResult.get(), filestring.data(), filestring.size() + 1);

                        *files = Graphics::FileReader::getDirectoryContents(std::string(pathResult.get()));
                        break;
                    }
                    else
                    {
                        std::memcpy(pathResult.get(), filestring.data(), filestring.size() + 1);
                    }
                }
        }
    }

    ImGui::EndChild();



   

    ImGui::Separator();



    std::string resultFilename = std::filesystem::path(std::string(pathResult.get())).filename().string();
    std::memcpy(fileNameBuffer.get(), resultFilename.data(), resultFilename.size() + 1);
    
    ImGui::Text("Filename: "); ImGui::SameLine();
    ImGui::SetNextItemWidth(inputTextWidth);
    ImGui::InputText
    (
        ("##FileName" + Name).c_str(), 
        fileNameBuffer.get(), 
        256
        //directoryFilter(std::string(directoryBuffer.get()))
    );
    if (!wrapFooter) 
        ImGui::SameLine();
    else 
    {
        float totalButtonsWidth = (buttonWidth * 2) + spacingX;
        float alignmentIndent = ImGui::GetContentRegionAvail().x - totalButtonsWidth;
    
        if (alignmentIndent > 0.0f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + alignmentIndent);
    }

    //ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (buttonWidth * 2) - (ImGui::GetStyle().ItemSpacing.x * 2));
    if (ImGui::Button("Open", ImVec2(buttonWidth, 0))) 
    {
        *activeState.get() = false; 
    }
    //ImGui::SetCursorPosX(ImGui::GetWindowWidth() - buttonWidth - ImGui::GetStyle().ItemSpacing.x);
   
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(buttonWidth, 0))) 
    {
        std::memcpy(pathResult.get(), "/\0", 2);
        *activeState.get() = false; 
    }
    ImGui::End();
}









Graphics::ModelWindow::ModelWindow(
    std::string name,
    std::shared_ptr<Graphics::ComponentManager> componentManager,
    std::function<std::pair<bool, nlohmann::json>()> &getJSON,
    std::function<void(const char *string)> &addModel,
    std::shared_ptr<std::vector<std::filesystem::directory_entry>> files)
    : GUIComponent(name),
      GET_JSON(getJSON),
      ADD_MODEL(addModel),
      explorer(Graphics::FileExplorerWindow("", directoryBuffer, drawExplorer, files, true)),
      modelView(files)
{
    Name = name;
    auto pair = getJSON();
    sceneChanged = pair.first;
    jsonSceneGraph = pair.second;
    COMPONENT_MANAGER = componentManager;

};

void Graphics::ModelWindow::draw()
{
    //std::vector<std::string> files = Graphics::FileReader::getFolders(Directory);

    if (*drawExplorer.get()) explorer.draw();
    
    ImGui::Begin(Name.c_str());

    if(ImGui::Button(ICON_FA_FOLDER))
        *drawExplorer = true; 

    ImGui::SameLine();
    ImGui::InputText
    (
        "Directory", 
        directoryBuffer.get() + 1, 
        256
        //directoryFilter(std::string(directoryBuffer.get()))
    );
        
    ImGui::SameLine(); 
    if (ImGui::Button(ICON_FA_REDO))
    {
        *files = Graphics::FileReader::getDirectoryContents(std::string(directoryBuffer.get()));
    }
                            
    modelView.draw();
    








/*
    if (ImGui::Button("Load Models"))
        if (std::string(directoryBuffer.get()) != lastDirectory)
        {
            lastDirectory = std::string(directoryBuffer.get());
            files = Graphics::FileReader::getFolders(lastDirectory);
        }



    ImGui::Text("Loaded Models");

    for (std::string file : files)
    {
        ImGui::Text(file.c_str());

	// Change so scene file type is searched for and then loaded
	// into function ADD_MODEL
        if (ImGui::Button(("SPAWN MODEL##" + file).c_str()))
        {
            ADD_MODEL((file + "/scene.gltf").c_str());
        }
    }
*/
    auto pair = GET_JSON();

    //std::cout << pair.second.dump(4) << std::endl;
  /*
    if (ImGui::TreeNode("Scene"))
    {
        iterateGraph(pair.second);
        ImGui::TreePop();
    }
    // TODO: OPTIMIZE POINTER CREATION AND DELETION

  
    for (auto pointer : floatPointers)
    {
        if (!usedPointers.contains(pointer.first))
        {

            floatPointers.erase(pointer.first);
        }
    }
    */
    usedPointers.clear();
    floatPointers.clear();
    ImGui::End();
}

int Graphics::ModelWindow::directoryFilter(std::string string)
{
    return 0;
}

void Graphics::ModelWindow::drawNodeTransforms(Graphics::Entity entity, std::string key)
{
    bool changed = false; 
    auto transform = COMPONENT_MANAGER->get<Graphics::Transform>(entity);

 
    std::stringstream entityIDss; 
    entityIDss << "Entity ID: " << entity;
    ImGui::Text((entityIDss.str()).c_str());
    ImGui::Text("Position");

   

    std::string sliderKeyRoot = key + "-float" + "-";
    std::string floatKeyPositionX = sliderKeyRoot + "x";
    std::string floatKeyPositionY = sliderKeyRoot + "y";
    std::string floatKeyPositionZ = sliderKeyRoot + "z";

    if (floatPointers.count(floatKeyPositionX) == 0)
        floatPointers.emplace(floatKeyPositionX, std::make_shared<float>(transform.position.x));

    if (floatPointers.count(floatKeyPositionY) == 0)
        floatPointers.emplace(floatKeyPositionY, std::make_shared<float>(transform.position.y));

    if (floatPointers.count(floatKeyPositionZ) == 0)
        floatPointers.emplace(floatKeyPositionZ, std::make_shared<float>(transform.position.z));


    usedPointers.insert(floatKeyPositionX);
    usedPointers.insert(floatKeyPositionY);
    usedPointers.insert(floatKeyPositionZ);

    float preFloatX = *floatPointers.at(floatKeyPositionX).get();
    float preFloatY = *floatPointers.at(floatKeyPositionY).get();
    float preFloatZ = *floatPointers.at(floatKeyPositionZ).get();
auto m = transform.localTransform;
/*
std::string print = std::format(
        "[{:.2f}, {:.2f}, {:.2f}, {:.2f}]\n"
        "[{:.2f}, {:.2f}, {:.2f}, {:.2f}]\n"
        "[{:.2f}, {:.2f}, {:.2f}, {:.2f}]\n"
        "[{:.2f}, {:.2f}, {:.2f}, {:.2f}]",
        m[0][0], m[1][0], m[2][0], m[3][0],  // Row 1
        m[0][1], m[1][1], m[2][1], m[3][1],  // Row 2
        m[0][2], m[1][2], m[2][2], m[3][2],  // Row 3
        m[0][3], m[1][3], m[2][3], m[3][3]   // Row 4
    );

   ImGui::Text("TRANSFORM MATRIX");
   ImGui::Text(print.c_str());
       */ 

    ImGui::SliderFloat("X", floatPointers.at(floatKeyPositionX).get(), -100.0f, 100.0f);
    ImGui::SliderFloat("Y", floatPointers.at(floatKeyPositionY).get(), -100.0f, 100.0f);
    ImGui::SliderFloat("Z", floatPointers.at(floatKeyPositionZ).get(), -100.0f, 100.0f);
    
    if (preFloatX != *floatPointers.at(floatKeyPositionX).get())
    {
        transform.position.x = *floatPointers.at(floatKeyPositionX).get();
	changed = true;
    }

    if (preFloatY != *floatPointers.at(floatKeyPositionY).get())
    {
        transform.position.y = *floatPointers.at(floatKeyPositionY).get();
	changed = true;
    }   

    if (preFloatZ != *floatPointers.at(floatKeyPositionZ).get())
    {
        transform.position.z = *floatPointers.at(floatKeyPositionZ).get();
	changed = true;
    }

    if (changed)
    {

        transform.localTransform = glm::translate
            (
                glm::mat4(1.0f), 
                transform.position
            );

        COMPONENT_MANAGER->add(entity, transform);

    }
}


void Graphics::ModelWindow::iterateGraph(const nlohmann::json &json)
{
    std::regex self_regex("ENTITY-", std::regex_constants::ECMAScript | std::regex_constants::icase);

    //std::cout << json.dump(1) << std::endl;
    for (auto it = json.begin(); it != json.end(); ++it)
    {
        if (it->is_structured())
        {
            if (std::regex_search(it.key(), self_regex))
            {
		    //std::cout << it.dump(2) << std::endl;
		//if (ImGui::TreeNodeEx(it.key().c_str())) 
		    std::string name = it.value()["children"]["entity"]["name"];
		    std::string id = it.value()["children"]["entity"]["id"];
		    name = name + "##" + id;
                if (ImGui::TreeNodeEx(name.c_str())) 
                {


                    std::string entityString = it.value()["children"]["entity"]["id"];
                    Graphics::Entity entity;
                    std::stringstream entityStream(entityString);
                    entityStream >> entity;
 
                    
                    drawNodeTransforms(entity, it.key());
		    if (ImGui::TreeNodeEx(("Children##" + it.key()).c_str()))
		    {
		        iterateGraph(*it);

                        ImGui::TreePop();
		    }
                    ImGui::TreePop();
                }
            }
	    else
	    {
		iterateGraph(*it);
	    }
        }
	/*
        else
        {
            if (std::regex_search(it.key(), self_regex))
            {
                if (ImGui::TreeNodeEx(it.key().c_str())) 
                {
                    ImGui::Text("Position");
 
                    std::string entityString = it.value()["children"]["entity"]["id"];
 
                    Graphics::Entity entity;
                    std::stringstream entityStream(entityString);
 
                    entityStream >> entity;
 
                    drawNodeTransforms(entity, it.key());
                    iterateGraph(*it);
                    ImGui::TreePop();
                }
            }
 	}
	*/
    }
}

void Graphics::ModelWindow::handleInput()
{
}

Graphics::SceneWindow::SceneWindow(
    std::string name,
    Graphics::FrameBuffer *sceneBuffer,
    std::function<void(float, float)> &resizeFunction,
    std::vector<std::function<void()>> &functionStorage)
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

    std::function<void()> resizeBufferWindow = [this]()
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


