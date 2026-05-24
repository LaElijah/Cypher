#ifndef RENDERER_H
#define RENDERER_H

#include <GLFW/glfw3.h>
#include "Camera.h"
#include "ResourceManager.h"
#include "SystemManager.h"
#include "GLFWCanvas.h"
#include "Shader.h"
#include "GUI.h"
#include "RenderAPI.h"
#include <memory>
#include "ResourceManager.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "GUIComponent.h"
#include "FrameBuffer.h"
#include <functional>
#include "../../external/GLAD/glad.h"
#include <cstdlib>
#include <ctime>
#include <thread>


namespace Graphics
{

    class Renderer
    {
    public:
        Renderer();
        Renderer(std::shared_ptr<Graphics::GLFWCanvas> canvas);

        template <typename T>
        Renderer(
            std::shared_ptr<Graphics::GLFWCanvas> canvas,
            std::shared_ptr<Graphics::Camera> camera,
            std::shared_ptr<Graphics::GUI> gui,
            std::shared_ptr<Graphics::SystemManager> systemManager, 
            std::shared_ptr<Graphics::ResourceManager> resourceManager,
            Graphics::RenderAPI<T> &api)
            : Canvas(canvas),
              Camera(camera),
              GUI(gui),
              SystemManager(systemManager),
              ResourceManager(resourceManager)
        {

            ResourceManager->initialize();
            api.loadShaders(ResourceManager->getShaderInfo());

            GUI->initialize(Canvas->getWindow());
            run(api);
        }

        std::shared_ptr<Graphics::ResourceManager> getResourceManager();
        std::shared_ptr<Graphics::GLFWCanvas> getCanvas();
        std::shared_ptr<Graphics::Camera> getCamera();
        std::shared_ptr<Graphics::GUI> getGUI();

        void updateWindow(float width, float height);
        void shutdown();

        // API Dependent functions
        template <typename T>
        void draw(Graphics::RenderAPI<T> &renderAPI)
        {
            for (Graphics::RenderBatch &batch : batches)
            {
                auto shader = renderAPI.getShader(batch.shader);
                shader->use();

                shader->setUniform("model", glm::mat4(1.0f));
                shader->setUniform("view", Camera->getViewMatrix());
                shader->setUniform("projection", Camera->getProjectionMatrix());

                renderAPI.loadData(batch);
                renderAPI.drawElements(batch.counts.size());
                renderAPI.flush(batch);
            }
        }

        template <typename T>
        void run(Graphics::RenderAPI<T> &renderAPI)
        {
            // Initialize
            GLFWwindow *window = Canvas->getWindow();

	    renderAPI.createNamedBuffer(1000000 * 10 * sizeof(Graphics::Vertex), "vertices");
	    renderAPI.createNamedBuffer(1000000 * 10 * sizeof(Graphics::Vertex), "indices");
	    renderAPI.createNamedBuffer(10000 * sizeof(Graphics::Transform), "transforms");
	    renderAPI.createNamedBuffer(10000 * sizeof(uint64_t), "textureHandles");

            std::function<std::pair<bool, nlohmann::json>()> getJSON =
                [this]()
                {
                    return SystemManager->getJSONGraph();
                };
                    

            std::function<void(const char* string)> addModel = 
                [&renderAPI, this](const char* string)
                {
                    Graphics::ModelInfo info;
                    info.path = string;
		    Graphics::Entity modelEntity;
		    std::vector<Graphics::Entity> meshes;
		    int start = -1;
		    int size = 0;

		    auto modelLoader = SystemManager->getResourceManager();
		    // Is there a model? if not, load vertices into buffe
		    modelEntity = SystemManager->createModel();

		    if (modelRecords.count(info.path) == 0)
		    {
		        auto model = modelLoader->load(info);

		        for (Graphics::Mesh& mesh : model->meshes) // Gather mesh data for model 
		        {
			            std::shared_ptr<Graphics::BufferRecord> vRecord = renderAPI
			    .insert
			    (
			        mesh.vertices.data(), 
				mesh.vertices.size() * sizeof(Graphics::Vertex), 
				"vertices"
			    );

				    // TODO: Update so each entity
				    // gets a record
				    // for its mesh by appending
				    // entity at the end of the path
				    // or making a new object 
				    // type that replaces the
				    // info.path with 
				    // path and entity 
				    // prob the first option 
	            std::shared_ptr<Graphics::BufferRecord> iRecord = renderAPI
			    .insert
			    (
			        mesh.indices.data(), 
				mesh.indices.size() * sizeof(unsigned int), 
				"indices"
			    );

		    modelLoader->storeModelRecord(info.path, vRecord, iRecord);

			    SystemManager->createMesh(info, modelEntity);
			    //
			    //meshes.push_back(record);

			    /*
			    if (start = -1)
			        start = record->start;
			    else if (start > record->start)
				start = record->start;

			    size += mesh.vertices.size();
*/
		        }
		    }
		    else
		    {
		        for 
			(
			    std::shared_ptr<Graphics::BufferRecord> record : modelRecords.at(info.path)
			)
			    SystemManager->createMesh(info, modelEntity);

			
		    }


                    };
		   
		    //modelRecords.insert(info.path, meshes);
		    //modelCount.at(info.path) = modelCount.at(info.path) + 1;
                

            //ImGuiIO &io = GUI->getIO();
            Graphics::FrameBuffer *sceneBuffer = new Graphics::FrameBuffer(1920, 1080);

            std::function<void(float, float)> resizeFunction =
                [this, sceneBuffer](float width, float height)
            {
                sceneBuffer->updateWindowSize(width, height);
                updateWindow(width, height);
            };

            GUI->addEditorComponent(
                new Graphics::SceneWindow(
                    std::string("Scene"),
                    sceneBuffer,
                    resizeFunction,
                    PostRenderFunctions));

           GUI->addEditorComponent
               (new Graphics::ModelWindow
                   (
                       std::string("MODELS"), 
                       std::string("/home/personal/Projects/Cypher/data/Models"), 
                       SystemManager->getComponentManager(),
                       getJSON,
                       addModel
                   )
               );
            
               GUI->addComponent(new Graphics::TestWindow(std::string("WOAAH")));
            // Initialize End

            while (!glfwWindowShouldClose(window))
            {
                SystemManager->update();
                SystemManager->getRenderBatches(batches);
                
                processInput(window);

                renderAPI.clear();

                // For drawing to scene window
                // within gui

                if (GUI->isWindowed())
                    sceneBuffer->Bind();

                renderAPI.clear();
                draw(renderAPI);

                if (GUI->isWindowed())
                    sceneBuffer->Unbind();

                GUI->drawGUI();

                glfwPollEvents();
                glfwSwapBuffers(window);

                // Handle post render duties
                while (PostRenderFunctions.size() > 0)
                {
                    PostRenderFunctions.back()();
                    PostRenderFunctions.pop_back();
                }
                Canvas->updateDeltaTime();
            }

            GUI->shutdown();
            glfwTerminate();
        }

    private:
        float SCREEN_WIDTH = 1920;
        float SCREEN_HEIGHT = 1080;

        std::shared_ptr<Graphics::GUI> GUI;
        std::shared_ptr<Graphics::Camera> Camera;
        std::shared_ptr<Graphics::GLFWCanvas> Canvas;
        std::shared_ptr<Graphics::ResourceManager> ResourceManager;
        std::shared_ptr<Graphics::SystemManager> SystemManager;

        std::vector<std::function<void()>> PostRenderFunctions;

        void processInput(GLFWwindow *window);
        std::vector<Graphics::RenderBatch> batches;
	std::map<std::string, std::vector<std::shared_ptr<Graphics::BufferRecord>>> modelRecords;


	// TODO: TEMP MOVE TO RESOURCE MANAGER

    };
}

#endif
