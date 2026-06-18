#include "GUIChildComponent.h"
#include "imgui.h"
#include "IconsFontAwesome5.h"

Graphics::GUIChildComponent::GUIChildComponent(std::string name)
    : Name(name)
{
    
}


int Graphics::ModelView::childCount = 0;
std::vector<Graphics::ModelView::IconType> Graphics::ModelView::IconTypes = {LINE, SMALL, MEDIUM, LARGE};
void Graphics::ModelView::draw()
{

    ImGui::BeginChild(Name.c_str());

    std::string label;
    bool active = false; 

    for (IconType type : IconTypes)
    {
        if (IconSetting == type)
        {
            ImGui::PushID(type);
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.6f));
            active = true;
        }

        switch (type)
        {
            case LINE:
                label = "Line";
                break;
            case SMALL:
                label = "Small";
                break;
            case MEDIUM:
                label = "Medium";
                break;
            case LARGE:
                label = "Large";
                break;
        }
    
        if (ImGui::Button(label.c_str()))
            IconSetting = type;


        if (active)
        {
            ImGui::PopStyleColor();
            ImGui::PopID();
            active = false; 
        }
        if (type != LARGE) ImGui::SameLine();
    }

   

    if (files.get()->size() <= 0)
    {
        ImGui::Text("No Files Found");
    }
    else
    {
        std::string label;



        for(std::filesystem::directory_entry file : *files.get())
        {
            switch (IconSetting)
            {
                case LINE:
                    label = "file##" + file.path().string();
                    if (ImGui::Selectable((std::string(ICON_FA_FILE) + label).c_str()))
                    {}
                    ImGui::Text(label.c_str());
                    break;
                case SMALL:
                    label = "Small";
                    break;
                case MEDIUM:
                    label = "Medium";
                    break;
                case LARGE:
                    label = "Large";
                    break;
            }

        }
    }


    ImGui::EndChild();


}