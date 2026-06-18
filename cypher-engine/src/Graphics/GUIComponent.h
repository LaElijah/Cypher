#ifndef GUICOMPONENT_H
#define GUICOMPONENT_H

#include <iostream>
#include <functional>
#include <string>
#include "FrameBuffer.h"
#include "FileReader.h"
#include <nlohmann/json.hpp>
#include <set>
#include "ComponentManager.h"
#include "GUIChildComponent.h"
// abstract class

namespace Graphics
{
    /*
     * NOTE: GUI Components use virtual functions as I
     * dont expect high performance out of my debug menu
     */

    /**
     * This class provides an interface for subclasses
     * to inherit, providing GUI windows with specific
     * functionality to easily be renderable to the GUI.
     *
     */
    class GUIComponent
    {
    public:
        GUIComponent(std::string name);
        std::string getName();
        // virtual ~GUIComponent();
        virtual void draw() = 0;
        virtual void handleInput() = 0;

    protected:
        std::string Name;

    };

    class TestWindow : public GUIComponent
    {
    public:
        TestWindow(std::string name);
        void draw() override;
        void handleInput() override;
    };

  class FileExplorerWindow: public GUIComponent
    {
        public:
            FileExplorerWindow
            (
                std::string path, 
                std::shared_ptr<char[]> pathResult,
                std::shared_ptr<bool> activeState,
                std::shared_ptr<std::vector<std::filesystem::directory_entry>> files, 
                bool directoriesOnly
            );

            void draw() override;
            void handleInput() override;
        private: 
            std::string path;
            std::shared_ptr<char[]> pathResult; 
            std::unique_ptr<char[]> fileNameBuffer;
            static int windowCount;
            std::shared_ptr<bool> activeState; 
            std::shared_ptr<std::vector<std::filesystem::directory_entry>> files;
            bool directoriesOnly;
    };

    class ModelWindow : public GUIComponent
    {
    public:
        ModelWindow(
            std::string name,
            std::shared_ptr<Graphics::ComponentManager> componentManager,
            std::function<std::pair<bool, nlohmann::json>()>& getJSON,
            std::function<void(const char *string)>& addModel,
            std::shared_ptr<std::vector<std::filesystem::directory_entry>> files
        );

        void draw() override;
        void handleInput() override;


    private:
        Graphics::ModelView modelView;
        std::shared_ptr<std::vector<std::filesystem::directory_entry>> files = std::make_shared<std::vector<std::filesystem::directory_entry>>();
        std::shared_ptr<char[]> directoryBuffer = std::make_unique<char[]>(256);
        std::function<void(const char *string)> ADD_MODEL;
        std::function<std::pair<bool, nlohmann::json>()> GET_JSON;
        int directoryFilter(std::string string);
        std::shared_ptr<bool> drawExplorer = std::make_shared<bool>(false); 
        Graphics::FileExplorerWindow explorer;

	    void drawNodeTransforms(Graphics::Entity entity, std::string key);
        void iterateGraph(const nlohmann::json& json);
        nlohmann::json jsonSceneGraph;
        bool sceneChanged = true;
        std::unordered_map<std::string, std::shared_ptr<float>> floatPointers;
        std::set<std::string> usedPointers;
        std::shared_ptr<Graphics::ComponentManager> COMPONENT_MANAGER;
    };

  

    class SceneWindow : public GUIComponent
    {
    public:
        SceneWindow(
            std::string name,
            Graphics::FrameBuffer *sceneBuffer,
            std::function<void(float, float)> &resizeFunction,
            std::vector<std::function<void()>> &functionStorage);

        void draw() override;
        void handleInput() override;

    private:
        Graphics::FrameBuffer *SceneBuffer;
        std::function<void(float, float)> &resizeWindow;
        std::vector<std::function<void()>> &postRenderFunctions;
    };

/*
Project window shows all the loaded assets
(models, materials, bones, etc) with a folder tree for each asset 
allow for the creation of new folders that represent each model 
and a file explorer window to load new models 
the folder created will be the place where models 
can be loaded from to create new entities 

in wide panel mode: 
left hand, the asset folder directory tree 
center, grid list of assets for given model 

in grid list 
*/
    //class ProjectWindow

/*
Inspector Window
*/
}

#endif
