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

Graphics::ModelWindow::ModelWindow(
    std::string name,
    std::string directory,
    std::shared_ptr<Graphics::ComponentManager> componentManager,
    std::function<std::pair<bool, nlohmann::json>()> &getJSON,
    std::function<void(const char *string)> &addModel)
    : GUIComponent(name),
      GET_JSON(getJSON),
      ADD_MODEL(addModel)
{
    Name = name;
    Directory = directory;
    auto pair = getJSON();
    sceneChanged = pair.first;
    jsonSceneGraph = pair.second;
    COMPONENT_MANAGER = componentManager;
};

void Graphics::ModelWindow::draw()
{
    std::vector<std::string> files = Graphics::FileReader::getFolders(Directory);
    ImGui::Begin(Name.c_str());

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

    auto pair = GET_JSON();

    //std::cout << pair.second.dump(4) << std::endl;

    if (ImGui::TreeNode("Scene"))
    {
        iterateGraph(pair.second);
        ImGui::TreePop();
    }
    // TODO: OPTIMIZE POINTER CREATION AND DELETION

    /*
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


void Graphics::ModelWindow::drawNodeTransforms(Graphics::Entity entity, std::string key)
{
    bool changed = false; 
    auto transform = COMPONENT_MANAGER->get<Graphics::Transform>(entity);

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
