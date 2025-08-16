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
#include "../../external/imgui/imgui.h"
#include "../../external/imgui/backends/imgui_impl_opengl3.h"
#include "../../external/imgui/backends/imgui_impl_glfw.h"
#include "GUIComponent.h"
#include "FrameBuffer.h"
#include <functional>
#include "../../external/GLAD/glad.h"

// POTENTIAL NAME CYPHER;

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




                std::cout << "GETTING SHADER" << std::endl;
                auto shader = renderAPI.getShader(batch.shader);

                shader->use();

                std::cout << "USING SHADER" << std::endl;

                shader->setUniform("model", glm::mat4(1.0f));
                shader->setUniform("view", Camera->getViewMatrix());
                shader->setUniform("projection", Camera->getProjectionMatrix());


                std::cout << "LOADING DATA" << std::endl;
                renderAPI.loadData(batch);

                std::cout << "DRAWING DATA" << std::endl;
                renderAPI.drawElements(batch.counts.size());

                renderAPI.flush(batch);
            }
        }

        template <typename T>
        void run(Graphics::RenderAPI<T> &renderAPI)
        {
            // Initialize
            GLFWwindow *window = Canvas->getWindow();

            std::function<void(const char* string)> addModel = 
                [this](const char* string)
                {
                    std::cout << "ADD MODEL " << string << std::endl;
                    Graphics::ModelInfo info;
                    info.path = string;
                    Graphics::Entity entity = SystemManager->createModel(info);

                };
            ImGuiIO &io = GUI->getIO();
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
                       std::string("/Users/Games/Documents/c++/Cypher/data/Models"), 
                       addModel
                   )
               );
            GUI->addComponent(new Graphics::TestWindow(std::string("WOAAH")));
            // Initialize End

            while (!glfwWindowShouldClose(window))
            {

                SystemManager->update();
                
                SystemManager->getRenderBatches(batches);


                if (true)
                {
                    processInput(window);
                }

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

						std::cout << "AFTER TRAVERSAL" << std::endl;
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

        std::vector<Graphics::Vertex> vertexData;
        std::vector<unsigned int> indexData;
        std::vector<int> counts;

        std::vector<const void *> indices;
        std::vector<int> baseVertices; // Current vertex offset per mesh

        unsigned int currentBaseIndex = 0;
        int currentBaseVertex = 0;
        unsigned int instanceIndex = 0;

        std::vector<Graphics::ElementDrawCall> drawCalls;

        std::string shaderName;
        glm::mat4 modelMatrix;
        bool done = false;
    };
}

#endif
