#ifndef RENDERER_H
#define RENDERER_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "ResourceManager.h"
#include "GLFWCanvas.h"
#include "Shader.h"
#include "GUI.h"
#include "RenderAPI.h"
#include <memory>
#include "ResourceManager.h"
#include "Model.h"
#include "Mesh.h"
#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_opengl3.h"
#include "../../external/imgui/imgui_impl_glfw.h"
#include "GUIComponent.h"
#include "FrameBuffer.h"
#include <functional>



// POTENTIAL NAME CYPHER;


namespace Graphics 
{



    class Renderer 
    {
	public:
            Renderer();
            Renderer(std::shared_ptr<Graphics::GLFWCanvas> canvas);

	    template <typename T>
            Renderer
	    (
		std::shared_ptr<Graphics::GLFWCanvas> canvas,
		std::shared_ptr<Graphics::Camera> camera,
		std::shared_ptr<Graphics::GUI> gui,
		std::shared_ptr<Graphics::ResourceManager> resourceManager,
		Graphics::RenderAPI<T>& api
	    )
                : Canvas(canvas),
		  Camera(camera),
		  GUI(gui), 
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
            void draw(Graphics::RenderAPI<T>& renderAPI)
            {
                //for (std::shared_ptr<Graphics::Model> model : ResourceManager->getLoadedModels())
                for (Graphics::Model* model : ResourceManager->getLoadedModels())
                { 
	            auto shader = renderAPI.getShader(model->getShaderName());
                    shader->use();
                    shader->setUniform("model", model->getModelMatrix());      
                    shader->setUniform("view", Camera->getViewMatrix());
                    shader->setUniform("projection", Camera->getProjectionMatrix());
                    

                    for (Graphics::Mesh& mesh : model->getMeshes())
                    {
            	        renderAPI.loadData(mesh.getVertices(), mesh.getIndices(), model->getShaderName()); 
                        // Move to implementation in api 
	   		//ResourceManager->loadTextures(mesh.getTextures());
            
	   		renderAPI.loadTextures(mesh.getTextures());
            	        renderAPI.drawElements(mesh.getIndices().size());	
            	    }
                }
            }
 
	    template <typename T>
            void run(Graphics::RenderAPI<T>& renderAPI)
            {
		// Initialize
                GLFWwindow *window = Canvas->getWindow();
                ResourceManager->loadModel(new Graphics::Model(
					"/home/laelijah/Gengine/data/Models/room/scene.gltf"));
            
                ImGuiIO& io = GUI->getIO();
                Graphics::FrameBuffer* sceneBuffer = new Graphics::FrameBuffer(1920, 1080);
            
                std::function<void(float, float)> resizeFunction = 
			[this, sceneBuffer] 
			(float width, float height) 
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
            
                GUI->addEditorComponent(new Graphics::TestWindow(std::string("Test")));
                GUI->addComponent(new Graphics::TestWindow(std::string("WOAAH")));
		// Initialize End
                
	        // Render loop	
                while (!glfwWindowShouldClose(window)) 
                {   
                    if (true)
                    {
                        processInput(window);
                    }
            
                    if (GUI->isWindowed())
            	    {	
            	        sceneBuffer->Bind();  
            	        
		        renderAPI.clear(); 
            	        draw(renderAPI); 
                        
		        sceneBuffer->Unbind(); 	    
            	        
		        GUI->drawGUI();
                    }

            	    else 
            	    {	
            	        draw(renderAPI);  
            	        GUI->drawGUI();
            	    }
            
            	    glfwPollEvents();
            	    glfwSwapBuffers(window);
                    
		    // Clear default frame buffer 	
            	    renderAPI.clear();
            
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

            std::vector<std::function<void()>> PostRenderFunctions;
	    
            void processInput(GLFWwindow *window);

    };
}


#endif
